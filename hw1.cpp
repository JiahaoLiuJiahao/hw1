#include <iostream>
#include <fstream>
#include <cstdio>
#include <cassert>
using namespace std;

int size;
bool **image;

void renderPixel(int x, int y) {
	assert(x >= 0 && y >= 0 && x <= size && y <= size); //Ensure both x, y >= 0 because it is in first quadrant and <= size to not go over the canvas.
	image[x][y] = 1;
	image[y][x] = 1;			            //Need to 1/8 circle to finish a quarter circle and these two 1/8 are reflections over y=x.
}

void rasterizeArc(int radius) {
	int x=0;		
	int y=radius;		//Draw a 1/8 circle, from (0,radius) to (radius/sqrt(2),radius/sqrt(2)), then draw its reflection over y=x to finish the quarter of circle in first quadrant.
	int d = 1 - radius;	//Decision parameter d, initially set to (1-radius), deciding if we should choose E or SE.
	renderPixel(x,y);	//Draw the first point on the starting point (0, radius) and its reflection over y=x, which is (radius,0).

	while(y>x)		//Ensure y>x such that points won't go across x axis.
	{
		if(d<0)			//Middle point lays inside circle
			d+=2*x+3;	//Choose E, offset = d(new)-d(old)=2xi+3;
		else{
			d+=2*(x-y)+5;	//Choose SE, offset = d(new)-d(old)=2xi-2yi+5. (By defult, choose SE if middle:wq point lays in the circle)
			y--;		//If choose SE (inside the circle), the next position should go down to avoid moving away from center.
		}
		x++;			//x always goes up by 1 no matter how d is.
		renderPixel(x,y);	//Draw a pixel in current position. (Either E or SE)
	}
}

int main(int argc, char *argv[]) {
	if (argc != 2) {
		cout << "Usage: " << argv[0] << " circleSize\n";
		return 0;
	}
	
#ifdef _WIN32
	sscanf_s(argv[1], "%d", &size);
#else
	sscanf(argv[1], "%d", &size);
#endif
	if (size <= 0) {
		cout << "Image must be of positive size.\n";
		return 0;
	}
	
	// reserve image as 2d array
	image = new bool*[size+1];
	for (int i = 0; i <= size; i++) image[i] = new bool[size+1];
	
	rasterizeArc(size);
	
	char filename[50];
#ifdef _WIN32
	sprintf_s(filename, 50, "circle%d.ppm", size);
#else
	sprintf(filename, "circle%d.ppm", size);
#endif
	
	ofstream outfile(filename);
	outfile << "P3\n# " << filename << "\n";
	outfile << size+1 << ' ' << size+1 << ' ' << 1 << endl;

	for (int i = 0; i <= size; i++)
	for (int j = 0; j <= size; j++)	
		outfile << image[size-i][j] << " 0 0\n";
	
	// delete image data
	for (int i = 0; i <= size; i++) delete [] image[i];
	delete [] image;
	
	return 0;
}

