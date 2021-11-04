DEF_CMD_(hlt, 0x00, 0, 
{
    printf ("Program has finished!\n");
    return OK;
})

DEF_CMD_(push, 0x01, 1,
{
    PushStack (softCPU->st, val);
})

DEF_CMD_(pop, 0x02, 1, 
{
    topVal = PopStack (softCPU->st);
})

DEF_CMD_(add, 0x03, 0, 
{
    topVal = PopStack (softCPU->st);
    prevTopVal = PopStack (softCPU->st);

    PushStack (softCPU->st, topVal + prevTopVal);
})

DEF_CMD_(mul, 0x04, 0, 
{
    topVal = PopStack (softCPU->st);
    prevTopVal = PopStack (softCPU->st);

    PushStack (softCPU->st, topVal * prevTopVal);
})

DEF_CMD_(sub, 0x05, 0,
{
    topVal = PopStack (softCPU->st);
    prevTopVal = PopStack (softCPU->st);

    PushStack (softCPU->st, topVal - prevTopVal);
})

DEF_CMD_(out, 0x06, 0,
{
    topVal = PopStack (softCPU->st);

    printf ("Top stack value: %lg\n", topVal);

    continue;

})

DEF_CMD_(in, 0x07, 0,
{
    double tmpVal = 0;
    printf ("Enter a real number: ");
    while (scanf ("%lg", &tmpVal) == 0) {
        printf ("Please, enter real value!\n");
        while (getchar () != '\n') ;
    }

    PushStack (softCPU->st, tmpVal);
})

DEF_CMD_(sqrt, 0x08, 0, 
{
    topVal = PopStack (softCPU->st);

    PushStack (softCPU->st, sqrt (topVal));
})

DEF_CMD_(div, 0x09, 0, 
{
    topVal = PopStack (softCPU->st);
    prevTopVal = PopStack (softCPU->st);

    PushStack (softCPU->st, topVal / prevTopVal);
})

DEF_CMD_(cos, 0x0A, 0, 
{
    topVal = PopStack (softCPU->st);

    PushStack (softCPU->st, cos (topVal));
})

DEF_CMD_(sin, 0x0B, 0, 
{
    topVal = PopStack (softCPU->st);

    PushStack (softCPU->st, sin (topVal));
})

DEF_CMD_(jmp, 0x0C, 2,
{
    int index = softCPU->ip;
    softCPU->ip = *(u_int16_t *)(softCPU->machineCode + index - 1);
})

DEF_CMD_(je, 0x0D, 2,
{
    topVal = PopStack (softCPU->st);
    prevTopVal = PopStack (softCPU->st);

    if (fabs(topVal - prevTopVal) < 1e-6) {
        int index = softCPU->ip;
        softCPU->ip = *(u_int16_t *)(softCPU->machineCode + index - 1);
    }
})

DEF_CMD_(jne, 0x0E, 2,
{
    topVal = PopStack (softCPU->st);
    prevTopVal = PopStack (softCPU->st);

    if (fabs(topVal - prevTopVal) > 1e-6) {
        int index = softCPU->ip;
        softCPU->ip = *(u_int16_t *)(softCPU->machineCode + index - 1);
    }
})

DEF_CMD_(ja, 0x0F, 2,
{
    topVal = PopStack (softCPU->st);
    prevTopVal = PopStack (softCPU->st);

    if (topVal > prevTopVal) {
        int index = softCPU->ip;
        softCPU->ip = *(u_int16_t *)(softCPU->machineCode + index - 1);
    }
})

DEF_CMD_(dec, 0x10, 0,
{
    topVal = PopStack (softCPU->st);

    PushStack (softCPU->st, topVal - 1);
})

DEF_CMD_(ret, 0x11, 0,
{
    ;
})