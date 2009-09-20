/************************************************************************************************

fourierVision - Computer Vision for the RobotCub iCub Humanoid Robot
  
Early code is based on Fourier segmentation, stereo, and optical flow techniques. 
Most of the theory underlying this implementation can be found in:
 
    Vernon, D. "Fourier Vision", Kluwer Academic Publishers, 2001
 
Later code is based on work done at Etisalat University College, UAE (www.euc.ae)
in particular the colour segmentation and head servo control code.

The most recent code is based on the log-polar transform developed at  
LIRA-Lab, DIST, University of Genoa  (www.liralab.it).

A version to allow use with the Yarp robot software platform (www.yarp.org)
can be created by undeleting the #define YARP line.
This version has to be compiled and linked with Cmake (the CMakeLists file is in the src directory)

Yarp has been adopted by RobotCub consortium (www.robotcub.org and www.icub.org)
as the platform on which to develop the cognitive systems software for the iCub.

This package was implemented by David Vernon
 
 
*************************************************************************************************/


#define DVINT   1
#define DVFLOAT 2


#define HUE_SAMPLES 360
#define SATURATION_SAMPLES 100

#define COLOUR_IMAGE        3 // colour mode -> 3 bytes per pixel
#define GREYSCALE_IMAGE     1 // Greyscale colour mode -> 1 byte per pixel

#define CARTESIAN2LOGPOLAR     0   // log-polar transform: cartesian to log-polar mode
#define LOGPOLAR2CARTESIAN     1   // log-polar transform: log-polar to cartesian mode

#define VIEW_LOGPOLAR         0   // log-polar transform: view log-polar image
#define VIEW_CARTESIAN        1   // log-polar transform: view cartesian reconstruction
#define LOG_POLAR_WIDTH       252 // number of angles
#define LOG_POLAR_HEIGHT      152 // number of rings
 
#define PIX(f,width,i,j)   (*((f) + ( (j) * (width) )  + (i) ))
#define _min(X, Y)  ((X) < (Y) ? (X) : (Y))

 
/************************************************************************************************

class definitions
 

 	   DVcamera
	   DVimage
	   DVdisplay

*************************************************************************************************/

 


/*---------------------------------------------------------------------------------------------*

class DVimage

Description: General purpose image, greyscale or colour (RGB)


Methods:     DVimage
             ~DVimage
             get_size
             get_image_type
             set_image_type
             read
             write
             get_pixel
             put_pixel
             read_annotation
             write_annotation
             contrast_stretch

  

DVimage(int w, int h, int mode, unsigned char *image=NULL, char *description=NULL, int type=DVINT) 

   default constructor

   w, h       - width and size of image (required arguments)
   mode       - defaults to GREYSCALE_IMAGE
   image      - image data (defaults to NULL otherwise must by of type unsigned char)
                if no data is provided, the constructor allocates space and initializes the image to 255
	    	    otherwise it does a deep copy of the argument;

		        We do this to avoid the overhead in copying image data which might be generated by 
		        functions which don't use DVimage(s).

   description - image annotation (defaults to NULL)
				 if no description is provided, the constructor allocates space for a string with 
				 one null character '\0' otherwise it does a DEEP copy of the argument

   type        - type of image to be represented; must be either unsigned char (DVINT) or float (DVFLOAT)

~DVimage()    destructor: deletes image data and annotation string
 


void get_size(int *w, int *h)

  finds the size of the image
 
  w, h         - return the width and height of the image
         


int get_image_type();

   returns the type of the image, either DVINT or DVFLOAT


int get_image_mode();

   returns the mode of the image, either GREYSCALE_IMAGE or COLOUR_IMAGE



void set_image_mode(int colour_mode)

    set the colour mode
  


void read(unsigned char *image)

   copy image data from object to argument; arguments as follows
  
   image - if not NULL, returns with a copy of the image data



void write(unsigned char *image)

   copy image data from argument to object; arguments as follows
  
   image - pointer to data to be copied to object

  
void read(float *image)

   copy image data from object to argument; convert from unsigned char to float
  
   image - if not NULL, returns with a copy of the image data



void write(float *image)

   copy image data from argument to object; convert from float to unsigned char
  
   image - pointer to data to be copied to object

                 
unsigned char get_pixel(int x, int y, int byte_number=0)

   read and return a single pixel value from the image
   note that the byte number needs to be specified if the mode is COLOUR_IMAGE 
   (it defaults to byte 0, i.e. the first byte)

   x, y  - width, height coordinates of the pixel
   byte_number - the byte number in a triplet for COLOUR_IMAGES (0, 1, or 2)
   method returns the pixel value



void put_pixel(int x, int y, unsigned char value, int byte_number=0) 

   x, y  - width, height coordinates of the pixel
   byte_number - the byte number in a triplet for COLOUR_IMAGES (0, 1, or 2)


   write a single pixel value to the image
   note that the byte number needs to be specified if the mode is COLOUR_IMAGE
   it defaults to byte 0, i.e. the first byte)



char *read_annotation()

   return pointer to the image annotation string
 
 

void write_annotation(char *description)	  

   write a string to the image annotation
 
   description - string to be written


void initialize()

   initialized entire image to zero


void contrast_stretch();

   constrast stretch an image in place; minimum and maximum values will be 0 and 255 after invoking this method


Public Members:  none

*----------------------------------------------------------------------------------------------*/

class DVimage {

   public:
	  
      DVimage(int w, int h, int mode=GREYSCALE_IMAGE, unsigned char *image=NULL, char *description=NULL, int type = DVINT);
	   ~DVimage();
	   void get_size(int *w, int *h);
      int get_image_type();
	   int get_image_mode();
      void set_image_mode(int mode);
	   void read(unsigned char *image);
	   void read(float *image);
	   void write(unsigned char *image);
	   void write(float *image);
	   void get_pixel(int x, int y, unsigned char *value, int byte_number=0);
	   void get_pixel(int x, int y, float *value, int byte_number=0);
	   void put_pixel(int x, int y, unsigned char value, int byte_number=0);
	   void put_pixel(int x, int y, float value, int byte_number=0);
	   char *read_annotation();
	   void write_annotation(char *description);
	   void initialize();
	   void contrast_stretch();


      //private:
      unsigned char *idata; // default holder for DVINT image type
	   float *fdata;         // holder for DVFLOAT image type
      int width, height;
      int colour_mode;      // COLOUR or GREYSCALE
	   char *annotation;
	   int image_type;       // DVINT or DVFLOAT
};


/*---------------------------------------------------------------------------------------------*

class DVhs_histogram

*----------------------------------------------------------------------------------------------*/


class DVhs_histogram {

   public:
	  
      DVhs_histogram(int h=HUE_SAMPLES, int s=SATURATION_SAMPLES);
	   ~DVhs_histogram();
	   void get_dimensions(int *h, int *s);
 	   void get_bin(int h, int s, int *value);
	   void put_bin(int h, int s, int value);
	   void increment_bin(int h, int s);
	   void initialize();

   //private:
      int *data;  
      int hue_dimension, saturation_dimension;
};
 
 
/************************************************************************************************

data-structure definitions
 
*************************************************************************************************/


typedef  struct {
   int x;
   int y;
   float value;
} maxima_data_type;



/*** Fourier vision prototypes ***/

void fourier_segmentation (DVimage *input_image_1, DVimage *input_image_2, DVimage *output_image_1, DVimage *output_image_2, DVimage *phase_output_image_1, DVimage *phase_output_image_2, double threshold, int filter_radius, int non_maxima_suppression_radius, double min_max_threshold, int mask, double mask_threshold);
void fft(float *image, float *real, float *imaginary, int width, int height, int direction);
void test_rlft3 (float *image, int width, int height) ;
double log_magnitude (double a_r, double a_i);
double _magnitude (double a_r, double a_i);
double log_magnitude (double a_r, double a_i);
void add_complex (double op1_r, double op1_i, double op2_r, double op2_i, double *result_r,  double *result_i);
void subtract_complex (double op1_r, double op1_i, double op2_r, double op2_i, double *result_r,  double *result_i);
void multiply_complex (double op1_r, double op1_i, double op2_r, double op2_i, double *result_r,  double *result_i);
void divide_complex (double op1_r, double op1_i, double op2_r, double op2_i, double *result_r,  double *result_i);
void power_complex (double op1_r, double op1_i, double op2, double *result_r,  double *result_i);
void exp_complex (double op1_r, double op1_i, double *result_r,  double *result_i);
void bilinear_interpolation_complex (float x_offset, float y_offset,double v00_r, double v00_i, double v01_r, double v01_i, double v10_r, double v10_i, double v11_r, double v11_i, double *result_r,  double *result_i);
void sort_fourier_components (float *ro1, float *io1, float *ro2, float *io2, float *po1, float *po2, int width, int height, int correct_phase, FILE *velocity_info_file, float *vel_x1, float *vel_y1, float *vel_x2, float *vel_y2);
//void solve_for_phasors_and_attenuate_sss (double ft0_r, double ft0_i, double threshold, int filter_order, double v_max, double *f1_r, double *f1_i, double *phi1_r, double *phi1_i);
int  plot_field(DVimage *f_mag, DVimage *f_phase, DVimage *gs_image, int gs_image_flag, DVimage **plot_image, float scale_factor, int colour, int superimpose);
int  interpolate(DVimage *sampled_image, DVimage **interpolated_image);
void dump_float_image (float *o, int width, int height);
void dump_int_image (int *o, int width, int height);
void dump_char_image (unsigned char *o, int width, int height);
int  draw_line(float *plot, int width, int x1, int y1, int x2, int y2, int colour);
void cross_power_spectrum (float *ri1, float *ii1, float *ri2, float *ii2, int width, int *x1, int *y1, int *x2, int *y2);
void cross_power_spectrum (DVimage *input_image_1, DVimage *input_image_2, DVimage *output_image);
void find_maxima (DVimage *input_image,  int number_of_maxima_required, int non_maxima_suppression_radius, maxima_data_type maxima[]);
void enhance_local_maxima(DVimage *input_image, int half_kernel_size, DVimage *output_image);
int  enhance_local_maxima_by_thinning(float *source_image, float *merged_image, int width, int height);
int  enhance_local_maxima_by_suppression(float *source_image, float *maxima_image, int width, int height);
int  enhance_local_maxima_by_filtering(float *source_image, int half_kernel_size, float *maxima_image, int width, int height);
void mask_image (DVimage *input_image, DVimage *mask_image,  DVimage *output_image, double threshold);
void colour_histogram (DVimage *input_image, DVhs_histogram *hs);
void gaussianApodization (DVimage *input_image, float std_dev, DVimage *output_image);
void rectify(DVimage *input_image_left, DVimage *input_image_right, 
             float fx_left,  float fy_left,  float px_left,  float py_left,  float theta_y_left,
             float fx_right, float fy_right, float px_right, float py_right, float theta_y_right,
             DVimage *output_image_left,  DVimage *output_image_right);

/*** Numerical Recipes in C prototypes ***/

#define TINY 1.0e-20
#define NR_END 1
#define FREE_ARG char*
#define REAL float

void gaussj(float **a, int n, float **b, int m);
void nrerror(char error_text[]);
int *ivector(long nl, long nh);
void free_ivector(int *v, long nl, long nh);
REAL *vector(long nl, long nh);
void free_vector(REAL *v, long nl, long nh);
REAL **matrix(long nrl, long nrh, long ncl, long nch);
void free_matrix(REAL **m, long nrl, long nrh, long ncl, long nch);
REAL ***f3tensor(long nrl, long nrh, long ncl, long nch, long ndl, long ndh);
void free_f3tensor(REAL ***t, long nrl, long nrh, long ncl, long nch, long ndl, long ndh);
void print_matrix(REAL **m, long nrl, long nrh, long ncl, long nch);
void print_vector(REAL *v, long nrl, long nrh);
void fourn(REAL data[], unsigned  long nn[], int ndim, int isign);
void rlft3(REAL ***data, REAL **speq, unsigned long nn1, unsigned long nn2, unsigned long nn3, int isign);
void gaussj(float **a, int n, float **b, int m);


/* Include file for binary image blob analysis (or connectivity analysis)    */
/*                                                                           */
/* Author: David Vernon                                                      */
/* Date:   29 March 2004                                                     */



// The following macro is used to simplify access to an individual pixel component (red, green, 
// or blue) in the linear 1-D image data array d contained in an Fl_RGB_Image
// The pixel coordinates are (i,j) and the colour component is k (0 Red, 1 Green, 2 Blue)
//

#define pixel(d,width,i,j,k)   (*((d) + ( (j) * (width) * (3))  + (i) * (3) + (k) ))

 
#define MAX_BLOBS 10000                  //Maximum allowed blobs in a single image

#define MAX_NUMBER_OF_EQUIVALENCES 5000  // when doing connectivity analysis, we need to keep track of the equivalance
                                         // relationships between regions (multiple regions will make up a single 
                                         // connected blob).  These equivalenced are then resolved when labelling
                                         // the blobs, following which we compute the blob statistics
#define TRUE   1
#define FALSE  0
 

// blob_type data-structure

typedef struct {
  long int area;             // in pixels
  long int perimeter;        // in pixels
  long int centroid_x;  
  long int centroid_y;
  long int top_left_x;       // trivial minimum bounding rectangle (aligned with image axes) 
  long int top_left_y;
  long int bottom_right_x;
  long int bottom_right_y;  
} blob_type;


// prototypes

int add_equivalence(int label1, int label2, 
					int equivalence[][2], 
					int *ecount, 
					int max);

int length_of_perimeter  (int *blob_image, 
				          int image_size_x, int image_size_y,
						  int top_left_x, int top_left_y,
						  int blob_label);


void find_blobs  (unsigned char *binary_image, 
				  int image_size_x, int image_size_y, 
				  unsigned char *labelled_image,
				  blob_type blob_list[],  int *no_of_blobs);


