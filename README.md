# CHIP-8

 [![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)<br>
Yet another CHIP-8 interpreter written in C.

![chip8](images/chip8.png)

## Building & Running

### Dependencies
 * [SDL2](https://www.libsdl.org/)

```bash
$ cmake . && make
$ ./chip8 path/to/rom
```

## Controls

### CHIP-8 Keypad Layout
<table>
    <tr>
        <td>1</td>
        <td>2</td>
        <td>3</td>
        <td>C</td>
    </tr>
    <tr>
        <td>4</td>
        <td>5</td>
        <td>6</td>
        <td>D</td>
    </tr>
    <tr>
        <td>7</td>
        <td>8</td>
        <td>9</td>
        <td>E</td>
    </tr>
    <tr>
        <td>A</td>
        <td>0</td>
        <td>B</td>
        <td>F</td>
    </tr>
</table>

### Mapped To 
<table>
    <tr>
        <td>1</td>
        <td>2</td>
        <td>3</td>
        <td>4</td>
    </tr>
    <tr>
        <td>Q</td>
        <td>W</td>
        <td>E</td>
        <td>R</td>
    </tr>
    <tr>
        <td>A</td>
        <td>S</td>
        <td>D</td>
        <td>F</td>
    </tr>
    <tr>
        <td>Z</td>
        <td>X</td>
        <td>C</td>
        <td>V</td>
    </tr>
</table>

ESC - Close program

## Acknowledgements

 * [Included ROMs](https://github.com/kripod/chip8-roms)
 * [Cowgod's Chip-8 Technical Reference](http://devernay.free.fr/hacks/chip8/C8TECH10.HTM#8xy3)

## License

Distributed under the MIT License. See `LICENSE` for more information.
