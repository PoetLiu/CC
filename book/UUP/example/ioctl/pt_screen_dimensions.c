#include <sys/ioctl.h>
#include <stdio.h>

void print_screen_dimensions()
{
	struct winsize wbuf;

	if (ioctl(0, TIOCGWINSZ, &wbuf) != -1) {
	
		printf("%d rows x %d cols\n", wbuf.ws_row, wbuf.ws_col);
		printf("%d wide x %d tall\n", wbuf.ws_xpixel, wbuf.ws_ypixel);
	}
}

int main(void)
{
	print_screen_dimensions();
	return 0;
}
