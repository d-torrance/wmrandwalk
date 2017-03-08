/* wmrandwalk - Window Maker dockapp displaying a random walk
 * Copyright (C) 2017 Doug Torrance <dtorrance@piedmont.edu>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301,
 * USA.
 */

#include <time.h>
#include <X11/Xlib.h>
#include <libdockapp/dockapp.h>

#define PARTICLE_COLOR "light sea green"
#define DOCKAPP_WIDTH 56
#define DOCKAPP_HEIGHT 56
#define TIMEOUT 100

GC particle_gc;
Pixmap pixmap;
int particle_x, particle_y;

void init_walk(int button, int state, int x, int y);
void do_walk(void);

enum {NORTH, EAST, SOUTH, WEST};

int main(int argc, char **argv)
{
	XGCValues values;
	DACallbacks eventCallbacks = {NULL,
				      init_walk,
				      NULL, NULL, NULL, NULL,
				      do_walk};

	srand(time(NULL));

	DAParseArguments(argc, argv, NULL, 0,
			 "Window Maker dockapp displaying a random walk",
			 PACKAGE_STRING);
	DAInitialize(NULL, PACKAGE_NAME, DOCKAPP_WIDTH, DOCKAPP_HEIGHT,
		     argc, argv);
	DASetCallbacks(&eventCallbacks);
	DASetTimeout(TIMEOUT);

	pixmap = DAMakePixmap();

	values.foreground = DAGetColor(PARTICLE_COLOR);
	particle_gc = XCreateGC(DADisplay, pixmap, GCForeground, &values);

	init_walk(0, 0, 0, 0);

	DAShow();
	DAEventLoop();

	return 0;
}

void init_walk(int button, int state, int x, int y)
{
	(void) button;
	(void) state;
	(void) x;
	(void) y;

	XFillRectangle(DADisplay, pixmap, DAGC, 0, 0,
		       DOCKAPP_WIDTH, DOCKAPP_HEIGHT);
	particle_x = rand() % DOCKAPP_WIDTH;
	particle_y = rand() % DOCKAPP_HEIGHT;
	XDrawPoint(DADisplay, pixmap, particle_gc, particle_x, particle_y);
	DASetPixmap(pixmap);
}

void do_walk(void)
{
	int direction;

try_again:
	direction = rand() % 4;
	switch (direction) {
		 case NORTH:
			 if (particle_y == 0)
				 goto try_again;
			 particle_y--;
			 break;
		 case EAST:
			 if (particle_x == DOCKAPP_WIDTH-1)
				 goto try_again;
			 particle_x++;
			 break;
		 case SOUTH:
			 if (particle_y == DOCKAPP_HEIGHT-1)
				 goto try_again;
			 particle_y++;
			 break;
		 case WEST:
			 if (particle_x == 0)
				 goto try_again;
			 particle_x--;
			 break;
	}

	XDrawPoint(DADisplay, pixmap, particle_gc, particle_x, particle_y);
	DASetPixmap(pixmap);
}
