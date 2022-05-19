int collisionMap[50][50] = 
{{100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100},
{100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100},
{100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,0,100,100,100,100,100,100,0,100,100,0,0,0,0,0,0,0,100,0,0,0,100,100,100,100,0,0,0,0,0,0,0,0,100,100},
{100,100,100,100,100,100,100,100,100,100,100,100,100,100,0,100,100,100,100,100,100,100,0,0,0,0,0,0,0,0,0,0,100,0,0,0,100,100,100,100,0,0,0,0,0,0,0,0,100,100},
{100,100,100,100,100,100,100,100,0,100,100,0,0,0,0,8,8,8,8,8,8,0,0,0,0,0,100,100,0,0,0,0,100,0,0,0,100,100,100,100,0,0,0,0,0,0,0,0,100,100},
{100,100,0,0,0,0,0,0,0,0,0,0,0,0,8,8,8,8,8,8,8,0,0,0,0,0,100,100,0,0,0,0,100,0,0,0,100,100,100,100,0,0,0,0,0,0,0,0,100,100},
{100,100,0,0,100,0,0,0,0,0,0,0,100,100,8,8,8,8,8,8,8,100,0,0,0,0,0,0,0,0,0,0,100,0,0,0,100,100,100,100,0,0,0,0,0,0,0,0,100,100},
{100,100,0,0,0,0,100,0,0,0,0,0,100,100,8,8,8,8,8,8,8,0,0,0,0,0,100,100,0,100,100,0,100,0,0,0,100,100,100,100,0,0,0,0,0,0,0,0,100,100},
{100,100,0,0,0,0,0,0,0,0,0,0,0,0,8,8,8,8,8,8,8,0,0,0,100,100,100,100,0,100,100,100,100,0,0,0,0,0,4,0,0,0,0,0,0,0,0,0,100,100},
{100,100,0,0,0,0,0,0,0,0,0,0,0,0,8,8,8,8,8,8,8,0,0,0,100,100,0,0,7,7,100,0,100,100,100,100,100,100,100,100,100,100,100,100,0,0,100,100,100,100},
{100,100,100,0,0,0,0,0,0,0,0,0,0,0,8,8,8,8,8,8,8,0,100,100,0,0,0,0,7,7,100,0,100,0,0,0,0,0,100,100,100,100,100,0,0,0,100,0,100,100},
{100,100,100,0,0,0,0,0,0,0,0,0,100,0,100,100,100,100,8,8,100,100,100,100,0,0,0,0,0,0,0,0,100,100,100,100,100,100,100,100,100,100,100,0,0,0,100,0,100,100},
{100,100,100,0,100,100,0,0,0,0,0,0,0,0,100,100,100,100,100,100,100,100,0,0,0,0,0,127,0,0,0,0,100,0,0,0,0,0,100,100,100,100,100,0,0,0,0,0,100,100},
{100,100,100,0,100,100,0,0,0,100,0,0,0,0,100,100,100,100,100,100,100,100,0,0,0,0,0,0,0,0,100,100,100,0,0,0,0,0,100,100,100,100,100,0,0,0,0,0,100,100},
{100,100,100,0,100,100,0,0,0,0,0,0,0,0,0,100,100,100,100,100,100,100,100,100,0,0,0,0,0,0,100,100,100,0,0,0,0,0,100,100,100,100,100,0,0,0,0,0,100,100},
{100,100,0,0,100,100,0,0,0,0,0,0,0,0,0,100,100,100,100,100,100,100,100,100,0,0,0,0,0,0,100,100,100,0,0,0,0,0,100,100,100,100,100,0,0,0,0,0,100,100},
{100,100,0,100,100,100,9,9,100,0,0,0,0,0,0,100,100,100,100,100,100,100,0,0,0,0,100,0,0,0,0,0,100,0,0,0,0,0,0,0,0,0,0,0,3,0,100,0,100,100},
{100,100,0,0,100,100,9,9,100,100,100,100,100,100,100,100,100,100,100,100,100,100,0,0,0,0,0,0,0,0,0,0,100,100,100,100,100,100,100,100,100,100,100,100,0,3,100,100,100,100},
{100,100,100,0,100,100,0,0,100,100,100,100,100,100,100,100,100,100,100,100,100,100,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,100,100},
{100,100,0,0,100,100,0,0,0,0,0,0,100,100,100,100,100,100,100,100,100,100,0,0,0,0,0,100,100,0,0,0,100,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,100,100},
{100,100,100,100,100,100,0,0,0,0,0,0,100,100,100,100,100,100,100,100,100,100,0,100,100,0,0,100,100,0,0,0,100,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,100,100},
{100,100,100,100,100,100,0,0,0,0,0,0,100,100,100,0,0,0,0,100,100,8,0,100,100,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,100,100},
{100,100,0,100,100,100,0,0,0,0,0,100,0,0,100,0,100,100,0,100,100,8,8,0,0,0,0,0,100,100,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,100,100},
{100,100,0,0,100,100,0,0,0,0,0,100,0,0,100,0,100,100,0,100,100,8,8,0,0,0,0,0,100,100,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,100,100,100,100},
{100,100,100,0,0,0,0,0,100,100,0,0,0,0,100,0,100,100,0,100,100,8,8,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,100,100,100,100},
{100,100,100,0,100,100,0,0,100,100,0,0,0,0,100,0,100,100,0,100,100,8,8,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,100,100,100,100},
{100,100,100,100,0,100,0,0,0,0,0,0,0,0,100,0,100,100,0,100,100,8,8,0,0,0,0,0,0,0,0,0,0,100,0,0,0,0,0,0,0,0,0,0,0,0,0,0,100,100},
{100,100,100,100,100,100,0,0,0,0,0,0,0,0,0,0,100,100,0,100,100,8,8,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,100,100},
{100,100,100,100,100,0,0,0,0,0,100,100,0,0,100,0,100,100,0,100,100,100,8,8,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,100,100},
{100,100,100,100,100,100,0,0,0,0,100,100,0,0,0,0,100,100,0,0,100,100,8,8,0,0,0,0,0,0,0,0,100,100,0,0,0,0,0,0,0,0,0,0,0,0,0,0,100,100},
{100,100,100,100,100,100,100,0,0,0,0,0,0,0,100,100,100,100,0,0,100,100,8,8,8,0,0,0,0,0,0,0,100,100,0,0,0,0,0,0,0,0,0,0,0,0,0,0,100,100},
{100,100,100,100,100,100,100,0,0,0,0,0,0,0,100,100,100,100,0,0,100,100,8,8,8,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,100,100},
{100,100,100,100,100,100,100,0,0,0,0,0,0,0,100,100,100,100,100,100,100,100,8,8,8,8,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,100,100},
{100,100,100,100,0,100,0,0,100,100,100,0,0,0,100,100,100,100,100,100,100,100,100,8,8,8,8,8,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,100,100},
{100,100,100,100,0,0,0,0,0,0,100,100,0,0,100,100,100,100,100,100,100,100,100,8,8,8,8,8,8,8,8,8,8,0,0,0,0,0,0,0,100,100,0,0,0,0,0,0,100,100},
{100,100,100,100,0,0,0,0,0,0,100,100,0,100,100,0,0,100,100,100,100,100,100,100,8,8,8,8,8,8,8,8,8,8,8,0,0,0,0,0,100,100,0,0,0,0,0,0,100,100},
{100,100,100,100,0,0,100,0,0,0,0,100,0,100,100,0,100,100,100,100,100,100,100,100,100,8,8,8,8,8,8,8,8,8,8,8,8,0,0,0,0,0,0,0,0,0,0,0,100,100},
{100,100,100,100,100,0,0,0,0,0,0,100,100,0,0,0,100,100,100,100,100,100,100,100,100,100,8,8,8,8,8,8,8,8,8,8,8,8,8,8,0,8,8,0,0,0,0,0,100,100},
{100,100,100,100,100,0,0,0,0,0,0,100,100,0,0,100,100,100,100,100,100,100,100,100,100,100,8,8,8,8,8,8,100,100,8,8,8,8,8,8,8,8,8,8,8,8,0,0,100,100},
{100,100,100,100,100,0,0,0,0,0,0,0,0,0,0,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,8,8,8,8,8,8,8,8,8,8,8,8,8,8},
{100,100,100,0,0,0,0,0,0,0,0,0,0,0,0,100,100,100,100,100,100,100,100,100,100,100,100,100,100,0,0,100,100,100,100,100,100,100,100,100,100,100,100,8,8,8,8,8,8,8},
{100,100,100,0,0,0,0,0,100,100,0,0,0,0,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,0,100,100,100,100,100,100,100,100,100,100,100,100,100,100,8,8,8,8,8,8},
{100,100,100,0,0,0,0,0,100,100,0,0,0,0,100,100,100,0,0,0,100,0,0,100,0,100,100,100,100,0,100,100,100,100,100,0,100,100,100,100,100,100,100,100,8,8,8,8,8,8},
{100,100,0,0,0,0,0,0,0,0,0,0,100,100,100,100,0,100,100,0,0,0,0,100,0,100,100,100,100,0,100,100,100,100,100,0,100,100,0,100,100,100,100,100,100,6,6,100,100,100},
{100,100,0,0,0,0,0,0,0,0,0,0,5,0,0,0,0,0,0,0,100,0,100,0,0,100,100,100,100,0,100,100,100,100,100,0,0,0,0,100,100,0,0,0,0,0,0,100,100,100},
{100,100,0,0,0,0,0,0,0,0,0,0,0,100,100,100,100,0,100,100,0,0,100,0,0,0,0,0,0,0,0,100,100,0,0,0,0,0,0,0,0,0,0,100,100,0,100,0,100,100},
{100,100,100,0,0,0,0,0,0,0,0,0,100,100,100,100,100,0,100,0,0,100,0,0,0,0,0,0,0,0,0,100,100,0,0,0,0,0,0,0,0,0,0,100,100,0,0,0,100,100},
{100,100,100,0,0,0,0,0,0,0,0,100,100,100,100,100,100,0,0,0,0,100,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,100,100},
{100,100,100,100,100,0,0,0,0,100,100,100,100,100,100,100,100,100,100,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,100,100,0,0,0,0,0,100,100,0,0,0,0,0,100,100},
{100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100}};
