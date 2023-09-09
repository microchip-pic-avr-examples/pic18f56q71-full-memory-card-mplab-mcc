/* 
 * File:   demoFileAssets.h
 * Author: C62081
 *
 * Created on September 8, 2023, 1:08 PM
 */

#ifndef DEMOFILEASSETS_H
#define	DEMOFILEASSETS_H

#ifdef	__cplusplus
extern "C" {
#endif

    const static char demoInfo[] = "<!DOCTYPE html><html><head><title>FatFs on a Memory Card with PIC18F56Q71</title></head><body><a href=microchip.com><svg version=1.1 width=10% viewBox=\"0 0 432 155\" xmlns=http://www.w3.org/2000/svg><style type=text/css>.st0{fill:#fff}.st1{fill:#ed1c2e}</style><path class=st0 d=\"m86.9 66.2c-4.26-1.22-8.77-1.88-13.4-1.88-25.4 0-46 19.4-46 43.4 0 24 20.6 43.4 46 43.4 23.4 0 42.7-16.4 45.6-37.7l-32.2-47.3z\"/><path class=st1 d=\"m92.6 68.3c-5.93-2.8-11.4-4.18-18.5-4.31-25.4-0.44-46.4 18.8-46.8 42.9-0.07 4.18 0.48 8.24 1.56 12.1l13.1-18.4 8.9 13.9s2.67 3.03 0.65 6.1c-0.13 0.19-12.1 16.7-12.1 16.7 8.25 8.64 19.9 13.9 33.1 14.2 12.8 0.23 24.3-4.46 32.8-12.4l-28-44.1s4.41-6.19 7.86-11.1c3.8-5.4 7.21-1.63 8.35 0.16 8.2 12.8 23.6 37.2 23.6 37.2l0.01-0.14c1.58-3.62 2.38-4.72 2.55-9.94l-27.2-42.9zm-6.63 52.8s-7.44 10.8-8.2 11.9c-4.55 6.18-7.9-0.96-7.9-0.96l-23.7-37.2s4.41-6.18 7.86-11.1c3.79-5.4 7.21-1.63 8.35 0.16 8.19 12.8 23.6 37.2 23.6 37.2z\"/><path d=\"m172 123-2.2-27.6h-0.09l-8.5 22c-0.79 2.05-2.53 3.73-4.97 3.73s-4.37-2.03-4.96-3.73l-8.31-21.7h-0.09l-2.39 27.3-8.11-0.01 4.14-38.8c0.27-2.14 2.03-4.59 4.42-4.59 3.67 0 5.14 2.15 5.87 4.29l9.39 23.6h0.09l9.38-23.6c0.83-2.14 2.25-4.29 5.87-4.29 2.39 0 4.15 2.44 4.42 5.09l4.1 38.4h-8.06z\"/><polygon points=\"184 90.2 192 90.2 192 123 184 123\"/><polygon points=\"363 90.2 371 90.2 371 123 363 123\"/><path d=\"m196 102c0-9.62 3.68-11.9 9.87-11.9h8.08c9.78 0 11-0.64 11 8.67l-17.7-0.03c-3.62 0-4.17 2.3-4.17 6.38v3.54c0 4.76 0.68 6.58 3.93 6.58l18.2-0.01c0 5.54-0.07 8.46-6.97 8.46h-12.4c-6.19 0-9.87-2.27-9.87-11.9v-9.81z\"/><path d=\"m258 102v-2.09c0-5.9-1.42-9.76-6.71-9.76h-22v32.9h6.87v-8.53h12c2.14 0 2.81 1.14 2.81 4.58v3.95h6.87v-5.58c0-5.4-1.7-6.72-3.74-7.26v-0.09c3.16-1.1 3.84-3.68 3.84-8.08m-6.68 0.86c0 2.36-0.68 3.45-2.54 3.45h-12.6v-7.99h12c2.17 0 3.16 0.55 3.16 3.45v1.09z\"/><path d=\"m262 102c0-9.62 3.68-11.9 9.87-11.9h11c6.19 0 9.87 2.27 9.87 11.9v9.8c0 9.62-3.68 11.9-9.87 11.9h-11c-6.19 0-9.87-2.27-9.87-11.9v-9.8m7.06 6.67c0 4.76 0.68 6.58 3.93 6.58h8.79c3.25 0 3.93-1.82 3.93-6.58v-3.54c0-4.09-0.68-6.58-4.3-6.58h-8.04c-3.62 0-4.3 2.5-4.3 6.58v3.54h-0.01z\"/><polygon points=\"330 90.2 337 90.2 337 102 351 102 351 90.2 358 90.2 358 123 351 123 351 111 337 111 337 123 330 123\"/><path d=\"m396 90.2h-21.2v32.9h6.87v-8.26h16.2c4.86 0 6.31-5.63 6.31-10.5v-2.82c0-6.44-1.58-11.2-8.11-11.2m1.05 12.9c0 1.95-0.62 3.49-2.23 3.49h-13.2v-8.26h12.6c2.07 0 2.82 1.27 2.82 3.49v1.28z\"/><path d=\"m297 102c0-9.62 3.68-11.9 9.87-11.9h8.08c9.78 0 11-0.64 10.9 8.67l-17.7-0.03c-3.62 0-4.17 2.3-4.17 6.38v3.54c0 4.77 0.68 6.58 3.93 6.58l18.2-0.01c0 5.54-0.07 8.46-6.97 8.46h-12.4c-6.19 0-9.87-2.27-9.87-11.9v-9.81z\"/><path d=\"m101 70.9c0-2.02 1.64-3.51 3.57-3.51 1.92 0 3.55 1.49 3.55 3.51 0 2.04-1.64 3.52-3.55 3.52-1.94 0-3.57-1.49-3.57-3.52m3.57 2.93c1.59 0 2.85-1.25 2.85-2.94 0-1.67-1.26-2.92-2.85-2.92-1.61 0-2.87 1.26-2.87 2.92 0 1.7 1.26 2.94 2.87 2.94m-0.74-0.9h-0.62v-4.06h1.54c0.96 0 1.43 0.36 1.43 1.15 0 0.73-0.46 1.04-1.05 1.12l1.15 1.78h-0.69l-1.07-1.76h-0.71v1.77h0.02m0.73-2.28c0.52 0 0.98-0.04 0.98-0.66 0-0.5-0.45-0.59-0.88-0.59h-0.84v1.26h0.74v-0.01z\"/>"
"</svg></a><h1>FatFs on a Memory Card with PIC18F56Q71</h1><ul><li><a href=https://mchp.us/44JtKpo>View the Documentation and Source Code</a></li><li><a href=https://mchp.us/4633R4R>Learn about the PIC18F56Q71 Family of Microcontrollers</a></li><li><a href=https://mchp.us/3RePTbP>Discover the PIC18F56Q71 Curiosity Nano Evaluation Kit (EV01G21A)</a></li></ul></body></html>";

#ifdef	__cplusplus
}
#endif

#endif	/* DEMOFILEASSETS_H */

