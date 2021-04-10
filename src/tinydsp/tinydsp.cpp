#include "tinydsp.h"


/* 
In all cases:
    1. get unique pointer of function object of a filter, while constructing it with the given parameters
    2. get the buffer size from the constructed filter and its specified requirements to set it afterwards
    3. set buffer size
If stored in the stack, just store the functor itself
*/

int DSP::add_filter(Filter_Type type, double data[]) {

    int filter_buf_size = 0;

#ifdef TINYDSP_FILTERS_USES_STACK

    if(filter_index >= filters.size()) {

    filters[filter_index] = Filter_Op{type}; 

        switch (type) {

        //Too much repetition

            case TINYDSP_LOW_PASS:
                filters[filter_index] = Filter_Op(type, Filter_Func{LP(data[0], data[1])} );
                break;

            case TINYDSP_HIGH_PASS:
                filters[filter_index] = Filter_Op(type, Filter_Func{HP(data[0], data[1])} );
                break;

            case TINYDSP_EQ:
                filters[filter_index] = Filter_Op(type, Filter_Func{Equalizer(data[0])} );
                break;

            case TINYDSP_DOWNMIX:
                filters[filter_index] = Filter_Op(type, Filter_Func{Downmix()} );
                break;

            case TINYDSP_GAIN:
                filters[filter_index] = Filter_Op(type, Filter_Func{Gain(data[0])} );
                break;

            case TINYDSP_LR_BALANCE:
                filters[filter_index] = Filter_Op(type, Filter_Func{LR_Balance(data[0])} );
                break;

            case TINYDSP_XFADE:
                filters[filter_index] = Filter_Op(type, Filter_Func{XFade(data[0])} );
                break;

            case TINYDSP_NO_FILTER:
            default:
                printf("Undefined or noop filter type at index %lu, not setting\n", filter_index);
                return TINYDSP_INVALID_FILTER;
        }

    } else {
        printf("Max amount of filters reached (%d), not adding\n", filters.size());
        return TINYDSP_ARRAY_FILLED;
    }

#else

    //Initialize with just type
    filters.emplace_back(Filter_Op{type});

    switch (type) {

        case TINYDSP_LOW_PASS:
            filters.back().filter_ptr = std::make_unique<LP>(data[0], data[1]);
            filter_buf_size = static_cast<LP*>(filters.back().filter_ptr.get())->out_coeffs;    // 2.
            break;

        case TINYDSP_HIGH_PASS:
            filters.back().filter_ptr = std::make_unique<HP>(data[0], data[1]);                     
            filter_buf_size = static_cast<HP*>(filters.back().filter_ptr.get())->out_coeffs;
            break;

        case TINYDSP_EQ:
            filters.back().filter_ptr = std::make_unique<Equalizer>(data);
            filter_buf_size = static_cast<Equalizer*>(filters.back().filter_ptr.get())->out_coeffs;
            break;

        case TINYDSP_DOWNMIX:
            filters.back().filter_ptr = std::make_unique<Downmix>();
            filter_buf_size = static_cast<Downmix*>(filters.back().filter_ptr.get())->out_coeffs;
            break;

        case TINYDSP_GAIN:
            filters.back().filter_ptr = std::make_unique<Gain>(data[0]);
            filter_buf_size = static_cast<Gain*>(filters.back().filter_ptr.get())->out_coeffs;
            break;

        case TINYDSP_LR_BALANCE:
            filters.back().filter_ptr = std::make_unique<LR_Balance>(data[0]);
            filter_buf_size = static_cast<LR_Balance*>(filters.back().filter_ptr.get())->out_coeffs;
            break;

        case TINYDSP_XFADE:
            filters.back().filter_ptr = std::make_unique<XFade>(data[0]);
            filter_buf_size = static_cast<XFade*>(filters.back().filter_ptr.get())->out_coeffs;
            break;

        case TINYDSP_NO_FILTER:
        default:
            printf("Undefined filter type at index %lu, removing\n", filters.size());
            filters.pop_back();
            return TINYDSP_INVALID_FILTER;
    }

    filters.back().buffer.resize(filter_buf_size); // 3.

#endif

}


