 /* sprites and sprite drawing */

//Player
char earthprotector[8][11] = {
	{0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 2, 2, 2, 0, 0, 0, 0},
	{0, 0, 0, 0, 2, 2, 2, 0, 0, 0, 0},
	{0, 2, 2, 2, 2, 2, 2, 2, 2, 2, 0},
	{2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2},
	{2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2},
	{2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2},
	{2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2}
};

char player_shoot[8][11] = {
    {7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
};


 //Alien 01 - Moving
 char alien01_moving_a[8][11] = {
	{0, 0, 7, 0, 0, 0, 0, 0, 7, 0, 0},
	{7, 0, 0, 7, 0, 0, 0, 7, 0, 0, 7},
	{7, 0, 7, 7, 7, 7, 7, 7, 7, 0, 7},
	{7, 7, 7, 0, 7, 7, 7, 0, 7, 7, 7},
	{7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7},
	{0, 7, 7, 7, 7, 7, 7, 7, 7, 7, 0},
	{0, 0, 7, 0, 0, 0, 0, 0, 7, 0, 0},
	{0, 7, 0, 0, 0, 0, 0, 0, 0, 7, 0}
};

char alien01_moving_b[8][11] = {
	{0, 0, 7, 0, 0, 0, 0, 0, 7, 0, 0},
	{0, 0, 0, 7, 0, 0, 0, 7, 0, 0, 0},
	{0, 0, 7, 7, 7, 7, 7, 7, 7, 0, 0},
	{0, 7, 7, 0, 7, 7, 7, 0, 7, 7, 0},
	{7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7},
	{7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7},
	{7, 0, 7, 0, 0, 0, 0, 0, 7, 0, 7},
	{0, 0, 0, 7, 7, 0, 7, 7, 0, 0, 0}
};


// Barrier
char barrier0bl[8][11] = {
	{0, 0, 0, 0, 2, 2, 2, 2, 2, 2, 2},
	{0, 0, 0, 2, 2, 2, 2, 2, 2, 2, 2},
	{0, 0, 2, 2, 2, 2, 2, 2, 2, 2, 2},
	{0, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2},
	{2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2},
	{2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2},
	{2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2},
	{2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2}	
};

char barrier0al[8][11] = {
	{2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2},
	{2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2},
	{2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2},
	{2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2},
	{2, 2, 2, 2, 2, 2, 0, 0, 0, 0, 0},
	{2, 2, 2, 2, 2, 0, 0, 0, 0, 0, 0},
	{2, 2, 2, 2, 0, 0, 0, 0, 0, 0, 0},
	{2, 2, 2, 2, 0, 0, 0, 0, 0, 0, 0}	
};

char barrier0ar[8][11] = {
	{2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2},
	{2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2},
	{2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2},
	{2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2},
	{0, 0, 0, 0, 0, 2, 2, 2, 2, 2, 2},
	{0, 0, 0, 0, 0, 0, 2, 2, 2, 2, 2},
	{0, 0, 0, 0, 0, 0, 0, 2, 2, 2, 2},
	{0, 0, 0, 0, 0, 0, 0, 2, 2, 2, 2}	
};
char barrier0br[8][11] = {
	{2, 2, 2, 2, 2, 2, 2, 0, 0, 0, 0},
	{2, 2, 2, 2, 2, 2, 2, 2, 0, 0, 0},
	{2, 2, 2, 2, 2, 2, 2, 2, 2, 0, 0},
	{2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 0},
	{2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2},
	{2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2},
	{2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2},
	{2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2}	
};