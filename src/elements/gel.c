#include <element.h>

int update_GEL(UPDATE_FUNC_ARGS) {
	int r, rx, ry;
	for (rx=-2; rx<3; rx++)
		for (ry=-2; ry<3; ry++)
			if (x+rx>=0 && y+ry>0 && x+rx<XRES && y+ry<YRES && (rx || ry))
			{
				r = pmap[y+ry][x+rx];
				if (!r)
					continue;

				//Desaturation
				if (((r&0xFF)==PT_WATR || (r&0xFF)==PT_DSTW || (r&0xFF)==PT_SLTW || (r&0xFF)==PT_CBNW)
				    && parts[i].tmp<100)
				{
					parts[i].tmp = (100+parts[i].tmp)/2;
					kill_part(r>>8);
				}

				char gel = 0;
				if ((r&0xFF)==PT_GEL)
					gel = 1;

				//Concentration diffusion
				if (gel && (parts[r>>8].tmp+1)<parts[i].tmp)
				{
					parts[r>>8].tmp++;
					parts[i].tmp--;
				}

				if ((r&0xFF)==PT_SPNG && (parts[r>>8].life+1)<parts[i].tmp)
				{
					parts[r>>8].life++;
					parts[i].tmp--;
				}

				float dx, dy;
				dx = parts[i].x - parts[r>>8].x;
				dy = parts[i].y - parts[r>>8].y;

				//Stickness
				if ((dx*dx + dy*dy)>1.5 && (gel || !ptypes[r&0xFF].falldown || (fabs(rx)<2 && fabs(ry)<2)))
				{
					float per, nd;
					nd = dx*dx + dy*dy - 0.5;

					per = 5*(1 - parts[i].tmp/100)*(nd/(dx*dx + dy*dy + nd) - 0.5);
					if (ptypes[r&0xFF].state==ST_LIQUID)
						per *= 0.1;
					
					dx *= per; dy *= per;
					parts[i].vx += dx; 
					parts[i].vy += dy; 

					if (ptypes[r&0xFF].properties&TYPE_PART || (r&0xFF)==PT_GOO)
					{
						parts[r>>8].vx -= dx;
						parts[r>>8].vy -= dy;
					}
				}
			}
	return 0;
}
