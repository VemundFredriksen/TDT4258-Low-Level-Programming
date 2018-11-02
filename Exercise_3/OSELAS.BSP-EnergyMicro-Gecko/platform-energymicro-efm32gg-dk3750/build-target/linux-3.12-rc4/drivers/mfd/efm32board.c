#include <linux/platform_device.h>
#include <linux/irq.h>
#include <linux/interrupt.h>
#include <linux/irqdomain.h>
#include <linux/export.h>
#include <linux/of_gpio.h>
#include <asm/io.h>

#define DRIVER_NAME "efm32board"

#define MAGIC		0x04
#define INTFLAG		0x40
#define INTEN		0x42

struct efm32board_ddata {
	void __iomem *base;
	unsigned int irq;
	struct irq_chip chip;
	struct irq_domain *domain;
};

static void efm32board_irq_ack(struct irq_data *data)
{
	struct efm32board_ddata *ddata = irq_get_chip_data(data->irq);
	unsigned short val;

	/* XXX: locking */
	val = readw(ddata->base + INTFLAG);
	val &= ~(1 << data->hwirq);
	writew(val, ddata->base + INTFLAG);
}

static void efm32board_irq_mask(struct irq_data *data)
{
	struct efm32board_ddata *ddata = irq_get_chip_data(data->irq);
	unsigned short val;

	if (data->hwirq != 2) {
		/* XXX: locking */
		val = readw(ddata->base + INTEN);
		val &= ~(1 << data->hwirq);
		writew(val, ddata->base + INTEN);
	}
}

static void efm32board_irq_unmask(struct irq_data *data)
{
	struct efm32board_ddata *ddata = irq_get_chip_data(data->irq);
	unsigned short val;

	/* XXX: locking */
	val = readw(ddata->base + INTEN);
	val |= 1 << data->hwirq;
	writew(val, ddata->base + INTEN);
}

static irqreturn_t efm32board_handler(int irq, void *data)
{
	unsigned short val;
	struct efm32board_ddata *ddata = data;
	irqreturn_t ret = IRQ_NONE;

	val = readw(ddata->base + INTFLAG);
	/* ack BC irq */
	writew(0, ddata->base + INTFLAG);

	pr_debug("%s: INTFLAG=%hx\n", __func__, val);

	while (val) {
		int line = __fls(val);

		if (!generic_handle_irq(irq_create_mapping(ddata->domain, line)))
			ret = IRQ_HANDLED;
		val &= ~(1 << line);
	}
	return ret;
}

int efm32board_irqdomain_map(struct irq_domain *d, unsigned int virq,
		irq_hw_number_t hw)
{
	struct efm32board_ddata *ddata = d->host_data;

	irq_set_chip_data(virq, ddata);
	irq_set_chip_and_handler(virq, &ddata->chip, handle_edge_irq);

	set_irq_flags(virq, IRQF_VALID);

	return 0;
}

const struct irq_domain_ops efm32board_irqdomain_ops = {
	.map = efm32board_irqdomain_map,
	.xlate = irq_domain_xlate_onecell,
};

static int efm32board_probe(struct platform_device *pdev)
{
	struct resource *res;
	int irq, gpio, ret;
	struct efm32board_ddata *ddata;
	unsigned short val;

	ddata = devm_kzalloc(&pdev->dev, sizeof(*ddata), GFP_KERNEL);
	if (!ddata) {
		dev_err(&pdev->dev, "cannot allocate driver data");
		return -ENOMEM;
	}

	res = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	if (!res) {
		dev_err(&pdev->dev, "can't get device resources\n");
		return -ENOENT;
	}

	gpio = of_get_named_gpio_flags(pdev->dev.of_node, "irq-gpios", 0, NULL);
	if (gpio < 0) {
		dev_err(&pdev->dev, "can't get irq gpio\n");
		return gpio;
	}

	ret = gpio_request(gpio, DRIVER_NAME);
	if (ret) {
		dev_err(&pdev->dev, "cannot request irq gpio\n");
		return ret;
	}

	ret = gpio_direction_input(gpio);
	if (ret) {
		dev_err(&pdev->dev, "cannot configure irq gpio as input\n");
		return ret;
	}

	irq = gpio_to_irq(gpio);
	if (irq <= 0) {
		dev_err(&pdev->dev, "can't get irq number\n");
		return irq < 0 ? irq : -ENOENT;
	}
	ddata->irq = irq;

	ddata->base = devm_request_and_ioremap(&pdev->dev, res);
	if (!ddata->base) {
		dev_err(&pdev->dev, "cannot request and ioremap register set\n");
		return -EADDRNOTAVAIL;
	}

	val = readw(ddata->base + MAGIC);
	if (val != 0xef32) {
		dev_err(&pdev->dev, "Magic not found (0x%hx)\n", val);
		return -ENODEV;
	}

	/* disable and clear all irqs */
	writew(0, ddata->base + INTEN);
	writew(0, ddata->base + INTFLAG);

	/* XXX: enable joystick irq */
	writew(4, ddata->base + INTEN);

	ddata->chip.name = DRIVER_NAME;
	ddata->chip.irq_ack = efm32board_irq_ack;
	ddata->chip.irq_mask = efm32board_irq_mask;
	ddata->chip.irq_unmask = efm32board_irq_unmask;

	ret = request_irq(irq, efm32board_handler, 0, DRIVER_NAME, ddata);
	if (ret)
		goto err_request_irq;

	ddata->domain = irq_domain_add_simple(pdev->dev.of_node, 5, 0,
			&efm32board_irqdomain_ops, ddata);
	if (!ddata->domain) {
		ret = -ENOMEM;
		dev_err(&pdev->dev, "cannot create irq domain\n");

		free_irq(irq, ddata);
	}
err_request_irq:
	return ret;
}

static const struct of_device_id efm32board_dt_ids[] = {
	{ .compatible = "efm32board", },
	{ /* sentinel */ }
};

static struct platform_driver efm32board_driver = {
	.probe = efm32board_probe,
	.driver = {
		.name = DRIVER_NAME,
		.owner = THIS_MODULE,
		.of_match_table = efm32board_dt_ids,
	},
};
module_platform_driver(efm32board_driver);
