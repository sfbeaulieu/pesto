#ifndef THREAD_H
#define THREAD_H

void acquisition(int *mode,int *loop,int *inc,struct disp display_struct);
void tcs(struct TCS *tcs_var);
void ccdTemp(struct camParam *detParam);
void getTempLoop(struct camParam *detParam);
void meteoThread(struct Meteo *meteo);
void isAcqOnGoing(int *ongoing);
void threadStop(int *close,int *isInAcq);
void getInc(int *incVar);
#endif // THREAD_H
