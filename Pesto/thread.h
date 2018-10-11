#ifndef THREAD_H
#define THREAD_H

void acquisition(int *mode,int *loop,int *inc);
void tcs(struct TCS *tcs_var);
void ccdTemp(struct camParam *detParam);
void getTempLoop(struct camParam *detParam);
void meteoThread(struct Meteo *meteo);
#endif // THREAD_H
