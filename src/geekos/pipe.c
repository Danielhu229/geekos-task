/*
 * Copyright (c) 2001,2003,2004 David H. Hovemeyer <daveho@cs.umd.edu>
 * Copyright (c) 2003,2013,2014 Jeffrey K. Hollingsworth <hollings@cs.umd.edu>
 *
 * All rights reserved.
 *
 * This code may not be resdistributed without the permission of the copyright holders.
 * Any student solutions using any of this code base constitute derviced work and may
 * not be redistributed in any form.  This includes (but is not limited to) posting on
 * public forums or web sites, providing copies to (past, present, or future) students
 * enrolled in similar operating systems courses the University of Maryland's CMSC412 course.
 */
#include <geekos/pipe.h>
#include <geekos/malloc.h>
#include <geekos/string.h>
#include <geekos/errno.h>
#include <geekos/projects.h>
#include <geekos/int.h>


struct File_Ops Pipe_Read_Ops =
    { NULL, Pipe_Read, NULL, NULL, Pipe_Close, NULL };
struct File_Ops Pipe_Write_Ops =
    { NULL, NULL, Pipe_Write, NULL, Pipe_Close, NULL };

static ulong_t get_remain_memmory_size(struct Pipe* p) {
    if (p->read_index > p->write_index) {
        return (p->read_index - p->write_index);
    } else {
        return (MAX_FIFO_FILE_SIZE + p->read_index - p->write_index + 1);
    }
}

int Pipe_Create(struct File **read_file, struct File **write_file) {
    struct Pipe* pipe = Malloc(sizeof (struct Pipe));
    pipe->buffer = Malloc(MAX_FIFO_FILE_SIZE);
    *read_file = Allocate_File(&Pipe_Read_Ops, 0, 0, (void*)pipe, 0, 0);
    *write_file = Allocate_File(&Pipe_Write_Ops, 0, 0, (void*)pipe, 0, 0);
    Print("Pipe_Create, write file:%p\n", *write_file);
    return 0;
}

int Pipe_Read(struct File *f, void *buf, ulong_t numBytes) {
    // TODO_P(PROJECT_PIPE, "Pipe read");

    return EUNSUPPORTED;
}

int Pipe_Write(struct File *f, void *buf, ulong_t numBytes) {
    // TODO_P(PROJECT_PIPE, "Pipe write");
    Print("call pipe write\n");
    struct Pipe* pipe = (struct Pipe*)(f->fsData);
    ulong_t max = get_remain_memmory_size(pipe);
    numBytes = numBytes > max ? max: numBytes;
    ulong_t i;
    char *dst = (char *)pipe->buffer;
    char *src = (char *)buf;
    for(i = 0; i < numBytes; ++i) {
        dst[pipe->write_index] = src[i];
        if (pipe->write_index == MAX_FIFO_FILE_SIZE) {
            pipe->write_index = 0;
        } else {
            pipe->write_index++;
        }
    }
    Print("write bytes:%lu\n", numBytes);
    return numBytes;
}

int Pipe_Close(struct File *f) {
    TODO_P(PROJECT_PIPE, "Pipe close");
    return 0;
}
