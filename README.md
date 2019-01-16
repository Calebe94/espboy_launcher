# espboy_launcher

![img](https://i.imgur.com/deGnXM5.png)

The [ESPBoy](https://github.com/Calebe94/ESPBoy) game [launcher](https://i.imgur.com/6rxyAPw.mp4).

# Setup

## On littlevgl pc_simulator

In the root folder of the pc_simulator do:

```
git clone https://github.com/littlevgl/pc_simulator
```

To test the launcher on the [pc_simulator](https://github.com/littlevgl/pc_simulator) you should edit some lines from [Makefile]() and [main.c]() files.

* In **Makefile** you should add in the end of the includes the following line:
```
    include ./espboy_launcher/sim_component.mk
```

* In **main.c** you edit the file as follow:
    * Add this in static variables:
    ```
        static lv_indev_t * indev;
    ```  
    * In hal_init() function remove every reference to mouse and add:
    ```
        keyboard_init();
        lv_indev_drv_t indev_drv;
        lv_indev_drv_init(&indev_drv);
        indev_drv.type = LV_INDEV_TYPE_KEYPAD;
        indev_drv.read = keyboard_read;
        indev = lv_indev_drv_register(&indev_drv);
    ```

# Team

| <img src="https://github.com/Calebe94.png?size=200" alt="Edimar Calebe Castanho"> | 
|:---------------------------------------------------------------------------------:|
| [Edimar Calebe Castanho (Calebe94)](https://github.com/Calebe94)                  |

# License

All software is covered under [MIT License](https://opensource.org/licenses/MIT).

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
