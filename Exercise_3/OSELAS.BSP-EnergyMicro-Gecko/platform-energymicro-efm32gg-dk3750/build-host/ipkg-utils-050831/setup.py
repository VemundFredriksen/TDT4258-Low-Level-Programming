import distutils.core

long_description = """
iPKG utilities implemented in Python

ipkg.py              implements Package class
"""

distutils.core.setup( name = 'ipkg-utils',
                      version = '1.7.3',
                      description = 'ipkg utilities implemented in python',
		      long_description = long_description,
                      author = 'Jamey Hicks',
                      author_email = 'jamey@handhelds.org',
		      license = 'GPL',
		      platforms = 'POSIX',
		      keywords = 'ipkg familiar',
                      url = 'http://www.handhelds.org/sources.html/',
                      py_modules = [ 'ipkg', 'arfile' ],
		      scripts = ['ipkg-compare-indexes', 'ipkg-make-index', 'ipkg-update-index', 'ipkg-build', 'ipkg-unbuild', 'ipkg-upload']
                      )
