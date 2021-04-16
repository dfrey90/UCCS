/*
Daniel Frey
CS2060_02
Assignment 2
9-7-17
This program runs a bookstore-- buy 1 type of book, process order, and generate order details
*/

#include <stdio.h>

const int TOTAL_MATH = 325, TOTAL_ENGLISH = 475, TOTAL_HISTORY = 1000;
const double MATH_PRICE = 37.5, ENGLISH_PRICE = 27.5, HISTORY_PRICE = 42;

//determine the shipping rate category
double detShipCategory(double reduction)
{
	if (reduction == 0)
		return 2.99;
	else if (reduction == 0.1)
		return 50;
	else if (reduction == 0.2)
		return 25;
	else
		return 0;
}

//determine the reduction percent
double detReductionCategory(int bkQnty)
{
	if (bkQnty < 21)
		return 0;
	else if (bkQnty < 51)
		return 0.1;
	else if (bkQnty < 101)
		return 0.2;
	else
		return 0.3;
}

//calculate total book cost
double totalBkCost(int bkOptn, int bkQnty)
{
	//book option already validated, if not 1 or 2, must be 3
	if (bkOptn == 1)
		return bkQnty * MATH_PRICE;
	else if (bkOptn == 2)
		return bkQnty * ENGLISH_PRICE;
	else
		return bkQnty * HISTORY_PRICE;
}

//check if any books need back ordering
int chkBackOrder(int bkOptn, int bkQnty)
{
	//must be book option 1, 2, or 3
	//if book need backorder--quantity is more than the total, return the positive amount
	//else none needed
	if (bkOptn == 1)
	{
		if (TOTAL_MATH - bkQnty < 0)
			return bkQnty - TOTAL_MATH;
		else
			return 0;
	}
	else if (bkOptn == 2)
	{
		if (TOTAL_ENGLISH - bkQnty < 0)
			return bkQnty - TOTAL_ENGLISH;
		else
			return 0;
	}
	else
	{
		if (TOTAL_HISTORY - bkQnty < 0)
			return bkQnty - TOTAL_HISTORY;
		else
			return 0;
	}
}

//generate the order details and display
void genOrdrDtls(int bkOptn, int bkQnty)
{
	double totalBkCst, reductionCat, shipCat, shipCost, taxes, amntDue, subTotal;
	int backOrder;

	//calculate parts of order details

	backOrder = chkBackOrder(bkOptn, bkQnty);

	totalBkCst = totalBkCost(bkOptn, bkQnty);

	reductionCat = detReductionCategory(bkQnty);
	shipCat = detShipCategory(reductionCat);

	//if shipping not flat rate--mult. to det. cost, else flat rate
	if (shipCat == 2.99)
		shipCost = bkQnty * shipCat;
	else
		shipCost = shipCat;

	subTotal = totalBkCst - (totalBkCst * reductionCat);
	taxes = subTotal * 0.075;
	amntDue = subTotal + shipCost + taxes;
	

	//display order details

	puts("\nBookstore Order Details");
		printf("   Number of books ordered     : %d\n", bkQnty);

	//print only if books must be back ordered
	if (backOrder > 0)
	{
		printf("   Number of books shipped     : %d\n", bkQnty - backOrder);
		printf("   Number of books back ordered: %d\n", backOrder);
	}

	printf("\n   Total Book Cost\t: $%.2f\n", totalBkCst);
	
	//if reduction then print stating which and amount
	if (reductionCat != 0)
		printf("   %.0lf%% Savings\t\t: $%.2lf\n", reductionCat * 100, totalBkCst * reductionCat);

	printf("   Shipping Cost\t: $%.2f\n", shipCost);
	printf("   Taxes\t\t: $%.2f\n", taxes);
	printf("-----------------------------------");
	printf("\n   Amount Due\t\t: $%.2f\n", amntDue);
}

//displays the bookstore menu and prompts user for selection and quantity
void displayBookstore()
{
	int bkOptn, bkQnty;

	//display bookstore menu
	puts("Welcome to the bookstore!\n");
	puts("Option \t Book \t\t Cost \t\t Available Copies\n");
	puts("----------------------------------------------------------");
	puts("1 \t Math \t\t $37.50 \t 325");
	puts("2 \t English \t $27.50 \t 475");
	puts("3 \t History \t $42.00 \t 1000");

	//prompt user for book selection
	printf("%s", "\nWhich book would you like to purchase? Select option 1, 2, or 3: ");
	scanf_s("%d", &bkOptn);

	//validate user selection
	if (1 <= bkOptn && bkOptn <= 3)
	{
		//prompt user for book quantity
		printf("%s", "How many copies do you want? ");
		scanf_s("%d", &bkQnty);

		//validate user specified quantity
		if (bkQnty > 0)
			//use book option and quantity to generate order details
			genOrdrDtls(bkOptn, bkQnty);
		else
			puts("\nYou've entered an invalid quantity, please run the program and try again.");
			
	}
	else
		puts("\nYou've entered an invalid menu option, please run the program and try again.");
}

int main(void)
{
	displayBookstore();

	return 0;
}