#pragma once

// Function to initialise RGB LED
void led_init();

// Function to stop color showing on RGB LED
void led_disable();

// Declare enum called color
typedef enum{
    red = 0,
    orange = 1,
    yellow = 2,
    green = 3,
    blue = 4, 
     
  
}color;

// Function to set the color of the RGB LED
void led_set_color(color led_color);
