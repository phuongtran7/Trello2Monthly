In order for the program to compile a `Secrets.h` file is needed. The content of that file should be:

```
#pragma once
static const utility::string_t trello_secrect(U("?key={YOUR KEY}&token={YOUR TOKEN}"));
```