# win32softwarecursor or dragging made right

### Intro

We often don't notice small issues, but eventually we become really disturbed because unconsciously we feel everything. One of those troubles is an input lag.

* **TODO** explain everything + gif's
* **TODO** low latency version on D3D

### How to use

* Just copy win32softwarecursor.c and win32softwarecursor.h to your project
* Call **win32softwarecursor(true)** before dragging starts
* Call **win32softwarecursor(false)** after dragging is done