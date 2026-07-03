

void adjust(int window_center,int window_width)
{
    int min = (2*window_center - window_width)/2.0 + 0.5;
    int max = (2*window_center + window_width)/2.0 + 0.5;
    int disp_pixel_val;
    int pixel_val;
    int nNumPixels;
    for (int i = 0; i != nNumPixels; i++)
    {
        disp_pixel_val = (pixel_val - min)*255.0/(double)(max - min);
    }
}
