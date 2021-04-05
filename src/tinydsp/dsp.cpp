#include "dsp.h"

DSP::DSP() {
}

int DSP::add_filter(Filter_Type type, double* data) {

    //if(filters.size >= MAX_FILTERS)

    filters.emplace_back(Filter_Type(type));
    int filter_buf_size = 0;

    switch (type) {

        /* 
        In all cases:
            1. get unique pointer of function object of a filter, while constructing it with the given parameters
            2. get the buffer size from the constructed filter and its specified requirements to set it afterwards
            3. set buffer size
        */

       //Too much repetition

        case DSP_LOW_PASS:
            filters.back().filter_ptr = std::make_shared<LP>(data[0], data[1]);                             // 1.
            filter_buf_size = std::static_pointer_cast<LP>(filters.back().filter_ptr).get()->out_coeffs;    // 2.
            break;

        case DSP_HIGH_PASS:
            filters.back().filter_ptr = std::make_shared<HP>(data[0], data[1]);                     
            filter_buf_size = std::static_pointer_cast<HP>(filters.back().filter_ptr).get()->out_coeffs;
            break;

        case DSP_EQ:
            filters.back().filter_ptr = std::make_shared<Equalizer>(data);
            filter_buf_size = std::static_pointer_cast<Equalizer>(filters.back().filter_ptr).get()->out_coeffs;
            break;

        case DSP_DOWNMIX:
            filters.back().filter_ptr = std::make_shared<Downmix>();
            filter_buf_size = std::static_pointer_cast<Downmix>(filters.back().filter_ptr).get()->out_coeffs;
            break;

        case DSP_GAIN:
            filters.back().filter_ptr = std::make_shared<Gain>(data[0]);
            filter_buf_size = std::static_pointer_cast<Gain>(filters.back().filter_ptr).get()->out_coeffs;
            break;

        case DSP_LR_BALANCE:
            filters.back().filter_ptr = std::make_shared<LR_Balance>(data[0]);
            filter_buf_size = std::static_pointer_cast<LR_Balance>(filters.back().filter_ptr).get()->out_coeffs;
            break;

        case DSP_XFADE:
            filters.back().filter_ptr = std::make_shared<XFade>(data[0]);
            filter_buf_size = std::static_pointer_cast<XFade>(filters.back().filter_ptr).get()->out_coeffs;
            break;

        case DSP_NO_FILTER:
        default:
            printf("Undefined filter type at index %lu, removing\n", filters.size());
            filters.pop_back();
            return DSP_INVALID_FILTER;
    }

    filters.back().buffer.resize(filter_buf_size); // 3.

    return 0;


}