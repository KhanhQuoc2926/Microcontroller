#ifndef INPUT_PROCESSING_H
#define INPUT_PROCESSING_H

/* Cac bien nay co the xem truc tiep trong Watch cua Keil. */
extern int mode;
extern int red_time;
extern int yellow_time;
extern int green_time;
extern int edit_value;
extern int phase;
extern int ns_count;
extern int ew_count;

void fsm_init(void);
void fsm_run(void);

#endif
