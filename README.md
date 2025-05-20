Changes needed for C89 compliance:
- Change `//` comments to `/*...*/`
- Move all variable declarations to the tops of functions and `{...}` blocks
- move `int i` out of `for (int i; ...)` statements; i.e., change to `int i; for (i = 0; ...)`
