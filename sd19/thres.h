#pragma once
#ifndef THRES_H_
#define THRES_H_

int find_first_thresh_forward(int* dat, int start, int datlen, int thresh, int* index, int* value);

int find_first_thresh_backward(int* dat, int start, int datlen, int thresh, int* index, int* value);
#endif