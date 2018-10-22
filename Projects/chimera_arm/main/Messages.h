#ifndef PROJECTS_ARM_MESSAGES_H_
#define PROJECTS_ARM_MESSAGES_H_

struct XhrStateMsg {
    int cam_id;
    int cam_shoulder;
    int cam_elbow;
    int gimbal;
    int base;
    int shoulder;
    int elbow;
    int wrist;
    int wrist_rot;
    int claw_open;
    int claw_close;
    int claw_torque;
};

struct SseHelloMsg {
};

static char *SseHelloFieldNames[] = {
};

struct SsePingMsg {
    int base;
    int shoulder;
    int elbow;
    int wrist_pitch;
    int wrist_rotation;
    int cam;
};

static char* SsePingFieldNames[] = {
    "base",
    "shoulder",
    "elbow",
    "wrist_pitch",
    "wrist_rotation",
    "cam",
    NULL,
};

#endif