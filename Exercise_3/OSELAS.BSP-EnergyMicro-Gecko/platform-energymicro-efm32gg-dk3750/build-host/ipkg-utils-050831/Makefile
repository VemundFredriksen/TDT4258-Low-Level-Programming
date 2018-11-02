UTILS = ipkg-build ipkg-upload

PREFIX=/usr/local

all: build

build: ipkg-compare-versions
	python setup.py build

ipkg-compare-versions: ipkg-compare-versions.c
	$(CC) $(CFLAGS) -o ipkg-compare-versions ipkg-compare-versions.c

install: ${UTILS}
	cp ${UTILS} $(PREFIX)/bin
	python setup.py install --prefix=$(PREFIX)
	chmod agu+rx ipkg-make-index
	cp -f ipkg-make-index $(PREFIX)/bin
	chmod agu+rx ipkg-list-fields
	cp -f ipkg-list-fields $(PREFIX)/bin

binary: build
	mkdir -p ipkg-build-binary/usr/bin
	cp ipkg-build ipkg-build-binary/usr/bin
	mkdir -p ipkg-build-binary/CONTROL
	cp CONTROL/control ipkg-build-binary/CONTROL
	chown -R root:root ipkg-build-binary
	ipkg-build ipkg-build-binary .
	rm -rf ipkg-build-binary

clean:
	rm -rf ipkg-build-binary ipkg-compare-versions *.pyc build
