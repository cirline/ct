#include <display.h>
#include <common.h>

#define SIG_LOW		"|    ""  "
#define SIG_HIGH	"|||||""  "
#define SIG_BOTH	"  x  ""  "
#define SIG_NONE	"  -  ""  "


void __print_hv_signal(int sig)
{
	switch(sig) {
		case HVS_LOW:
			printf(SIG_LOW);
			break;
		case HVS_HIGH:
			printf(SIG_HIGH);
			break;
		case HVS_BOTH:
			printf(SIG_BOTH);
			break;
		default:
			printf(SIG_NONE);
	}
}

void print_hv_signal(struct hv_signal *s)
{
	__print_hv_signal(s->vs);
	__print_hv_signal(s->hs);
	__print_hv_signal(s->de);
	__print_hv_signal(s->dclk);
	printf("\n");
}

void load_rgb_data(struct hv_signal *sigp)
{
}

void generate_hv_clk(struct hv_signal *sigp)
{
	sigp->dclk = HVS_HIGH;
	print_hv_signal(sigp);
	load_rgb_data(sigp);
	mdelay(400);
	sigp->dclk = HVS_LOW;
	print_hv_signal(sigp);
	mdelay(400);
}

int generate_hv_signal(struct hv_config *hvc)
{
	struct hv_signal sig;
	struct hv_config d;

	while(1) {
		int vspw = hvc->vspw;
		int frameline = hvc->frameline;
		int vbpd = hvc->vbpd;
		int vfpd = hvc->vfpd;
		int lineval = hvc->lineval;
		int vflag = 0;
		for(; frameline > 0; frameline--) {

			int linehoz = hvc->linehoz;
			int hspw = hvc->hspw;
			int hbpd = hvc->hbpd;
			int hfpd = hvc->hfpd;
			int hozval = hvc->hozval;

			if(vbpd > 0) {
				vflag = 0;
				vbpd--;
			} else if(lineval > 0) {
				vflag = 1;
				lineval--;
			} else if(vfpd > 0) {
				vflag = 0;
				vfpd--;
			}
			for(; linehoz > 0; linehoz--) {
				// vs signal
				if(vspw > 0) {
					sig.vs = HVS_LOW;
					vspw--;
				} else
					sig.vs = HVS_HIGH;

				// hs signal
				if(hspw > 0) {
					sig.hs = HVS_LOW;
					hspw--;
				} else
					sig.hs = HVS_HIGH;


				// de signal
				if(vflag) {
					if(hbpd > 0) {
						sig.de = HVS_LOW;
						hbpd--;
					} else if(hozval > 0) {
						sig.de = HVS_HIGH;
						hozval--;
					} else if(hfpd > 0) {
						sig.de = HVS_LOW;
						hfpd--;
					}
				} else
					sig.de = HVS_LOW;

				generate_hv_clk(&sig);
			}
		}
	}

	return 0;
}

