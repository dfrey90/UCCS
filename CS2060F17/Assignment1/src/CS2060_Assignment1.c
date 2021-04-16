/*
Daniel Frey
CS2060-02
Assignment 1
8-31-2017
This program will display information about myself. It will also
solve a 2x2 system of linear equations using Cramer's rule.
*/

#include <stdio.h>
#include <stdlib.h>

//display information about self
void displaySelfInfo(void)
{
	printf("\tMy name is Daniel Frey. I am from Westland, MI. I moved to Pueblo just \n"
			"over a year ago. I transferred to UCCS from the University of Michigan. At UofM \n"
			"I was studying computer science, just as I am doing here at UCCS. I am ranked \n"
			"as a junior. Most of my credits transferred from my previous college. This is \n"
			"my first semester at UCCS. I did most of my learning in C++. I am glad to learn \n"
			"a new programming language, I think it's fun! I have also learned Java, HTML, CSS\n"
			"JavaScript, and other associated web technology programming.\n\n"
			"\tMy favorite movies and books are of the Harry Potter series. I really enjoy \n"
			"the 6th movie, Half-Blood Prince. I like that this movie takes a darker tone when \n"
			"compared to the rest. For hobbies, I like to craft things. I like to make things \n"
			"with my hands. I've placed highly in cupcake competitions with my sister. I recently \n"
			"bought a Cricut personal die cutting machine. With that I can make things like window \n"
			"decals, clothing, glassware, and many more out of a variety of materials, such as \n"
			"vinyl or cardstock.");
}

//findDet calculates the determinant of a 2x2 matrix
float findDet(float a, float b, float c, float d)
{
	/* 2x2 matrix
	 * [a b]
	 * [c d]
	 * */
	return (a*d)-(c*b);
}

//takes values from a 2x2 system of equations and solves using Cramer's rule
void cramersRule(float a, float b, float c, float d, float e, float f)
{
	float x, y, det, detX, detY;

	//Cramer's rule:
	//det = ad-cb
	//x = Dx/D
	//y = Dy/D

	//finds the determinant of coefficient, x, and y matrices
	det=findDet(a,b,c,d);
	detX=findDet(e,b,f,d);
	detY=findDet(a,e,c,f);

	//scans special case, display solution
	if(det!=0 && detX==0 && detY==0)
		printf("x = 0, y = 0\n");
	else if(det!=0)
	{
		x=detX/det;
		y=detY/det;
		printf("x = %f, y = %f",x,y);
	}
	else
		puts("The system cannot be solved using Cramer's rule.");
}

int main(void)
{
	float a, b, c, d, e, f;

	//displays information about self
	displaySelfInfo();

	puts("\n\n\n");

	puts("Solving a 2x2 system of linear equations using Cramer's rule.");
	puts("Equations: 3.4x + 50.2y = 44.5 \n\t   2.1x + .55y  = 5.9");

	//assign values to variables
	a=3.4, b=50.2, e=44.5;
	c=2.1, d=.55, f=5.9;

	//calls Cramer's rule to solve and display solution
	cramersRule(a,b,c,d,e,f);

	/*
	x = ((e*d)-(b*f))/((a*d-b*c));
	y = ((a*f)-(e*c))/((a*d-b*c));
	*/

	return 0;
}
