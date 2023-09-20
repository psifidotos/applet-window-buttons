## How to add new local
1. Copy `template.pot` to `YOURLOCAL.po`, `YOURLOCAL` is system local. e.g. en_US, zh_CN...
2. Open the new .po file with `lokalize` or any text editor, begin translate.
3. Update and build translation with command `./merge && ./build` in this directory.
4. Open a new PR for your changes or open a new issue with the .po file you translated.

|  Locale  |  Lines  | % Done|
|----------|---------|-------|
| Template |      29 |       |
