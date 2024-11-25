use pin 10-15 to control 6 leds
gp20 interrupt to increase speed
gp22 interrupt to decrease speed
gp21 intterupt to insert next light

intiaite state: all off

when press gp21, 
    if off - (dim) set to 10% brightness
    if dim - set to full brightness
    if 100% - no change

previous led will turn off as sequence move forwards
start from gp10 led and move to next LED in the series
when all led reach  100% brightness, reset sequence and restart

gp20 increase speed by 100ms per press
gp22 decrease speed by 100ms per press

after all light up, press gp21 will reset

hi
