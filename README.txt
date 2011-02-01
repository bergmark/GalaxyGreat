== Getting started with GalaxyGreat:

* Open Map.SC2Map in the SC2 Map Editor
* Open the Import window (F9)
* Select the "Scripts" folder and press cmd+I (os x)/ctrl+I (win) and
  change the directory to the "scripts" folder in the repository
* All *.galaxy files should be selected, press OK, confirm the
  replacement
* Press cmd+F9/ctrl+F9 to run the map in SC2. If all goes well you
  should amongst other things have most of the map revealed (as in no
  fog of war mostly)

After making changes to the *.galaxy files in an external editor, go
through the import step again each time. (Hint: In OS X you can select
the import window and press cmd+I, RET, RET, cmd+S, cmd+F9 to import,
save and run SC2.

To make a custom map, leave `library.galaxy` as is and make
modifications in `map.galaxy`.

== Galaxy introduction:

* C derivative
* declare before define
* No dynamic/heap alloc
* No pointer arithmetic
* Functions may only return or recieve primitive atomic types, (may
  return void)
* Primitive types: int(size?), bool(true/false), string (?)
* Compound types: struct, array
* `static` for decs/defs local to file
* C-style `if`, `while`
* No `for` statement
* Must have braces around if/while body
* Reports of /= *= being buggy? (Unconfirmed on our end, may only have
  applied to the beta)
* View code editor generates by going into triggers > Data (menu) >
  view script / cmd+F11
* `// comments`
* /* comments not implemented */
* include directive `include "TriggerLibs/NativeLib"`
* C-style makes Emacs c-mode work well for editing.
