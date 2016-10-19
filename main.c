#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "algo.h"
#define NDIGITS 16

static const char *menu = "\n __________________________________________________________________\n1-Enter Number of routers\n2-Enter Cost between routers \n3-Build Routing Table for Each Router\n4-Out Optimal Path and Minimum Cost\n0-Exit\n\n you can use option number 1 to restart again :) \n __________________________________________________________________\n Your Choice-->";
static const char *prompt0 = "Invalid input. Please try again. Only character '1', '2', '3' or '4' is allowed.";
static const char *prompt1 = "Please enter number of routers";
static const char *prompt2 = "Please select a router:";
static const char *prompt3 = "Please input the source and destination router number:";

static int readNat(int *p) {
	static char buf[NDIGITS];
	static int r = 0;

	if (fgets(buf, NDIGITS, stdin) == NULL)
		return -1;
	if ((r = atoi(buf)) < 0)
		return -1;
	if (r == 0 && *buf != '0')
		return -1;
	
	return (*p = r);
}


int main() {
	char 	line[BUFSIZ];
	int 	row[BUFSIZ];

	int 	opt = 0;
	FILE*	file= NULL;
	//char*	fn 	= NULL;
	int 	ncol= 0;
	int 	tmp = 0;
	graph*	g 	= NULL;
	int*	ap 	= NULL;	// pointer within g->adjmtx
	router*	r 	= NULL;
	int 	fromto[2] = { 0, 0 };
	for (;;) {
		printf("%s", menu);
		switch (readNat(&opt)) {
		case 0:
			freegraph(&g);
			goto END;
		case 1:
			printf("%s\n", prompt1);
				ncol = readNat(&opt);;
				if (g != NULL)
					freegraph(&g);	// g == NULL
				g = makegraph(ncol);
				printf("Original routing table is as follows:\n");
				printgraph(g);
	
			break;
		case 2:
			{
				if((NULL == g) || (ncol==0))
				{
					printf("please initialize with option 1 first.\n");
					break;
				}
				int r1,r2,cost;
				printf("Original routing table is as follows:\n");
				printgraph(g);
				printf("\n\nEnter Source Router\n");
				r1= readNat(&opt);
				printf("Enter Destination Router\n");
				r2= readNat(&opt);
				printf("Enter Cost\n");
				cost= readNat(&opt);
				if((r1 < 0)||(r1 > ncol) || (r2 < 0)||(r2 > ncol) || (cost < -1))
				{
					printf("Invalid values please try again \n");
					break;
				}
				changeCostBetweenNodes(g, r1, r2, cost);
				
				printf("\nChanged routing table is as follows:\n");
				printgraph(g);
				
			}
			break;
		case 3:
			if (g == NULL) {
				printf("There is no data currently.\nPlease use option 1 \n");
				break;
			}
			printf("%s\n", prompt2);
			if (readNat(&tmp) < 0 || tmp > g->size) {
				printf("Not a valid router id.\n");
				break;
			}
			r = makerouter(tmp, g->size);
			buildtable(r, g);
			printf("The routing table for router %d is:\n", r->id);
			printrouter(r);
			freerouter(r);
			break;
		case 4:
			if (g == NULL) {
				printf("There is no data currently.\nPlease use option 1 \n");
				break;
			}
			{
				int r1,r2;
				printf("\n\nEnter Source Router\n");
				r1= readNat(&opt);
				printf("Enter Destination Router\n");
				r2= readNat(&opt);
				if((r1 < 0)||(r1 > ncol) || (r2 < 0)||(r2 > ncol))
				{
					printf("Invalid values please try again \n");
					break;
				}
				r = makerouter(r1, g->size);
				dijkstra(g, r->id, r->varray);
				printpath(r, r2);
				
			}	
			freerouter(r);
			break;
		default:
			printf("%s\n", prompt0);
			break;
		}
	}
END:
	return 0;
}
