
#include "Q_average_over_ensembles.h"
#include "cor_fft_c.h"
#include "mode_averaging_AC_CCs.h"
#include "ave_need_N1N2.h"
#include "calc_go_contour_source.h"

int main() {
    average_Q();
    perform_fft();
    mode_averaging();
    calculate_average();
    calculate_contour();

    return 0;
}
