#include <dispatch.h>

void dispatch_to_elevator(std::vector<elevator*> &elevators, buildingOfFloor *building)
{
    //上行分配
    if(building->request_queue_up.empty() == false)
    {
        int tar_up_num = building->request_queue_up.front();//楼层请求的层数
        //判断tar_up_num的有效性 （可能请求被其他路过电梯执行
        if(building->request_up[tar_up_num] == true)
        {
            int min_time = 2000;//初始化为一个较大的值
            unsigned int ele_num = 0;//分派任务的电梯号
            for(unsigned int i = 0; i < elevators.size(); i++)
            {
                if(cal_time(tar_up_num, elevators[i]) < min_time)
                {
                    min_time = cal_time(tar_up_num, elevators[i]);
                    ele_num = i;
                }
            }
            //委派任务
            elevators[ele_num]->ele_model_inside.request[tar_up_num] = true;
            //委派后删除请求
            building->request_queue_up.pop();
        }
        else
        {
            building->request_queue_up.pop();
        }
    }
    if(building->request_queue_down.empty() == false)
    {
        int tar_down_num = building->request_queue_down.front();
        if(building->request_down[tar_down_num] == true)
        {
            int min_time = 2000;
            unsigned int ele_num = 0;
            for(unsigned int i = 0; i < elevators.size(); i++)
            {
                if(cal_time(tar_down_num, elevators[i]) < min_time)
                {
                    min_time = cal_time(tar_down_num, elevators[i]);
                    ele_num = i;
                }
            }
            elevators[ele_num]->ele_model_inside.request[tar_down_num] = true;
            building->request_queue_down.pop();
        }
        else
        {
            building->request_queue_down.pop();
        }
    }
}



int cal_time(int target_floor, elevator *ele)
{
    //目标方向
    int tar_direct = 0;
    if(target_floor - ele->ele_model_inside.current_floor > 0)
        tar_direct = 1;
    else if (target_floor - ele->ele_model_inside.current_floor < 0)
        tar_direct = -1;

    //预测需要的总时间
    int sum_time = 0;

    //和运行方向相同
    if(tar_direct * ele->ele_model_inside.direction > 0)
    {
        for(int i = ele->ele_model_inside.current_floor; i != target_floor; i += tar_direct)
        {
            if(ele->ele_model_inside.request[i] == true)
            {
                sum_time += 7;
            }
            sum_time += 1;
        }
    }
    else if(tar_direct * ele->ele_model_inside.direction < 0)//和运行方向不同
    {
        //计算法反方向最远的那个楼层
        int furthest_floor = ele->ele_model_inside.current_floor;
        for(int i = ele->ele_model_inside.current_floor; i != 1 && i != 20; i += ele->ele_model_inside.direction)
        {
            if(ele->ele_model_inside.request[i] == true)
                furthest_floor = i;
        }
        //先加上从最远楼层返回当前楼层的时间
        sum_time += abs(ele->ele_model_inside.current_floor - furthest_floor);

        for(int i = furthest_floor; i != target_floor; i += tar_direct)
        {
            if(ele->ele_model_inside.request[i] == true)
            {
                sum_time += 7;
            }
            sum_time += 1;
        }

    }
    else
    {

        //等于零的情况 就在目标层数
        if(tar_direct == 0)
            sum_time = 0;
        else
        {
            for(int i = ele->ele_model_inside.current_floor; i != target_floor; i += tar_direct)
            {
                if(ele->ele_model_inside.request[i] == true)
                {
                    sum_time += 7;
                }
                sum_time += 1;
            }
        }

    }


    return sum_time;

}
