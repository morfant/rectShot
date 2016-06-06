n = NetAddr("localhost", 20000);
n.sendMsg("/laserPos", 100, 629);
n.sendMsg("/laserPos", 60, 440);
n.sendMsg("/laserPos", 98, 165);
n.sendMsg("/lasernPos", 139, 624);
n.sendMsg("/lasernPos", 320, 240);

n.sendMsg("/blackout", 1);
n.sendMsg("/firstShot", 0);


n.sendMsg("/lasernPos", 139, 624);



n.sendMsg("/button", 1);