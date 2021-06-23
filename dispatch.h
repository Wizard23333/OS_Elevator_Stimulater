#ifndef DISPATCH_H
#define DISPATCH_H

#include <elevator.h>
#include <buildingoffloor.h>

//估算电梯到目标层数的时间函数
int cal_time(int target_floor, elevator *ele);

//分派任务函数
void dispatch_to_elevator(std::vector<elevator*> &elevators, buildingOfFloor *building);


#endif // DISPATCH_H
