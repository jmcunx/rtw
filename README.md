## rtw - Remove Trailing Spaces

This is a little utility created a very long time
ago for systems without sed.  All it does is Remove
Trailing White space from a file.

To build, execute build.sh to generate a Makefile from
Makefile.template on most BSD/Linux systems and IBM AIX.  For MS-DOS,
it should be rather easy to create a Makefile.  I know it does compile
under MS-DOS using Microsoft c.

This requires [j_lib2](jmc.md) to build.

[GNU automake](https://en.wikipedia.org/wiki/Automake)
only confuses me, so I came up with my own method which
is a real hack.  But works for me.

This is licensed under the
[ISC Licence](https://en.wikipedia.org/wiki/ISC_license)

