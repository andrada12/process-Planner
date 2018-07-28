planificator: processPlanner.c
	gcc processPlanner.c -o planner -Wall


build:  planner



clean: 
	rm -f planner
