//
//  ImageDataSet.h
//  assimilateImage
//
//  Created by Marc Padró on 02/09/14.
//
//

#include "config.h"

#ifndef assimilateImage_ImageDataSet_h
#define assimilateImage_ImageDataSet_h

struct ImageDataSet
{
    
    const unsigned char* imageData;
    int pix_num;
    bool* removed;
    int reset_count = 0;
    int reset_value;
    
    
    ImageDataSet() : imageData(0), pix_num(0), removed(0), reset_value(DATASET_RESET)
    {
    }
    
    ImageDataSet(const unsigned char* data_ptr, int size, int a_reset_value = DATASET_RESET)
    {
        imageData = data_ptr;
        pix_num = size / 3;
        removed = new bool[pix_num];
        reset();
        reset_value = a_reset_value;
    }
    
    void set(const unsigned char* data_ptr, int size)
    {
        imageData = data_ptr;
        pix_num = size / 3;
        delete [] removed;
        removed = new bool[pix_num];
        reset();
    }
    
    inline void remove(int i)
    {
        if (i < pix_num)
        {
            if (++reset_count > reset_value) reset();
            else removed[i] = true;
        }
    }
    
    inline void reset()
    {
        int i = pix_num;
        while(i--)
        {
            removed[i] = false;
        }
        reset_count = 0;
        cout << "reset\n";
    }
    
    // Must return the number of data points
    inline size_t kdtree_get_point_count() const { return pix_num; }
    
    // Returns the distance between the vector "p1[0:size-1]" and the data point with index "idx_p2" stored in the class:
    inline int kdtree_distance(const unsigned char* p1, const size_t idx_p2, size_t size) const
    {
        
        const int i = idx_p2 * 3;
        
        if (true == removed[idx_p2])
        {
            //cout << "removed\n";
            return 999999;
        }
        
        const short d0 = p1[0] - imageData[i];
        const short d1 = p1[1] - imageData[i+1];
        const short d2 = p1[2] - imageData[i+2];
        return d0*d0 + d1*d1 + d2*d2;
    }
    
    // Returns the dim'th component of the idx'th point in the class:
    // Since this is inlined and the "dim" argument is typically an immediate value, the
    //  "if/else's" are actually solved at compile time.
    inline unsigned char kdtree_get_pt(const size_t idx, int dim) const
    {
        const int i = idx * 3;
        
        switch (dim) {
            case 0:
                return imageData[i];
                break;
            case 1:
                return imageData[i+1];
                break;
            case 2:
            default:
                return imageData[i+2];
                break;
        }
    }
    
    // Optional bounding-box computation: return false to default to a standard bbox computation loop.
    //   Return true if the BBOX was already computed by the class and returned in "bb" so it can be avoided to redo it again.
    //   Look at bb.size() to find out the expected dimensionality (e.g. 2 or 3 for point clouds)
    template <class BBOX>
    bool kdtree_get_bbox(BBOX &bb) const { return false; }
    
};

typedef nanoflann::KDTreeSingleIndexAdaptor<
nanoflann::L2_Simple_Adaptor<unsigned char, ImageDataSet, int> ,
ImageDataSet ,
3 /* dim */
> ImageKDTree;

#endif
