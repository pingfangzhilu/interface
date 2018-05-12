/* ------------------------------------------------------------------
 * Copyright (C) 2009 Martin Storsjo
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either
 * express or implied.
 * See the License for the specific language governing permissions
 * and limitations under the License.
 * -------------------------------------------------------------------
 */

#ifndef WAVREADER_H
#define WAVREADER_H
#ifdef __cplusplus
extern "C" {
#endif
#if 0
void* wav_read_open(const char *filename);
void wav_read_close(void* obj);

int wav_get_header(void* obj, int* format, int* channels, int* sample_rate, int* bits_per_sample, unsigned int* data_length);
int wav_read_data(void* obj, unsigned char* data, unsigned int length);
#endif

void* check_wavdata(const char *wavdata);

void wav_read_close_inter(void* obj);

int wav_get_header_inter(void* obj, int* format, int* channels, int* sample_rate, int* bits_per_sample, unsigned int* data_length);
int wav_read_data_inter(void* obj, char *wav_data, unsigned char** data, unsigned int length);


void* check_file(const char *infile);

int wav_read_file_inter(void* obj, unsigned char* data, unsigned int length);

#ifdef __cplusplus
}
#endif

#endif

