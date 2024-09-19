#include "TimerCondition.h"
#include "Agent.h"
#include "raylib.h"

bool TimerCondition::IsTrue(Agent* agent)
{
    timer -= GetFrameTime();
    if (timer <= 0)
    {
        timer = startTime;
        return true;
    }
    return false;
}
