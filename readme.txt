node.c operation instructions

After entering X Y when starting the program, a node map of X * Y size will be created.

Enter w, a, s, d to move around the map and display your current location.If there is no node in the direction of movement, it will not let you past.

Pressing r followed by w, a, s, d will delete the node in the direction of your choice.If there is no node in this direction, it cannot be deleted.

Press t and then enter the X and Y coordinates, the shortest path from the current position to the target coordinates will be displayed.
If the target location cannot be reached, it will display Unreachable.

Press q to end the loop, or close it directly, I did not free the node.

I am running with Dev C++.
===============================================================
hene2.py illustrate

This is the Tower of Hanoi solution using only one route, and due to the maximum depth limit, only a maximum of 9 discs can be run.

There are six kinds of moves each time. Analyze the situation of the last move. For example, it is impossible to move to the A column and immediately move from the A column to other columns. Therefore, it is possible to cut off two kinds of moves, and it is impossible to move both consecutively. Move from column A to column B, so delete one more possibility, and each move must leave three cases to analyze.

The rest depends on the situation, if the column to take the disc has no disc, or if the uppermost disc of the target column is smaller than the disc to be moved, it is also removed.

If there are still two moving situations that are legal, according to the observed rules, when the current disc to be moved and the last moved disc are not divisible by 2, the disc must be placed in the last moved disc. Above the disc, and vice versa, on another post.