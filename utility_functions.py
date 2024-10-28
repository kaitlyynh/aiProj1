import math

"""ACTIONS = {
    0: 0,
    2: 90,
    4: 180,
    6: 270,
    1: 45,
    3: 135,
    5: 225,
    7: 315
}"""

def cost_angle(curr_dir, action): # calculate delta theta
    diff = abs((curr_dir - action) * 45)
    if (diff > 180): diff = 360 - diff 
    return diff 


def cost_direction(action): # calculate c(s, a, s')
    if (action % 2): return 1
    else: return math.sqrt(2) 


def utility_func(s_curr, action, k): # entire utility function
    return k * cost_angle(s_curr, action) / 180 + cost_direction(action)
