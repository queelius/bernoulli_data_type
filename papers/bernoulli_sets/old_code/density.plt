set key on left top

# Each bar is half the (visual) width of its x-range.
#set boxwidth 0.05 absolute
#set style fill solid 1.0 noborder

bin_width = 0.001;

bin_number(x) = floor(x/bin_width)

rounded(x) = bin_width * ( bin_number(x) + 0.5 )

set xrange [:1]

#set title "Random positive predictive values.

set terminal latex
set output "density.tex"

#set terminal epslatex size 3.5,2.62 standalone color colortext 10
#set output 'example.tex'

set terminal svg
set output 'out.svg'

#stats 'ppv_1.dat'

# fpr=0.001, tpr=0.999, n=10000, and p=1000
# fpr=0.025, tpr=0.9, n=10000, and p=1000
# fpr=0.025, tpr=0.999, n=10000, and p=1000
# fpr=0.01, tpr=0.95, n=10000, and p=1000

plot 'ppv_1.dat' using (rounded($1)):(1.0/10000) smooth freq with boxes title "$\\fprate = 0.001$, $\\tprate=0.999$", \
    'ppv_2.dat' using (rounded($1)):(1.0/10000) smooth freq with boxes title "$\\fprate = 0.025$, $\\tprate=0.9$", \
    'ppv_3.dat' using (rounded($1)):(1.0/10000) smooth freq with boxes title "$\\fprate = 0.025$, $\\tprate=0.999$", \
    'ppv_4.dat' using (rounded($1)):(1.0/10000) smooth freq with boxes title "$\\fprate = 0.01$, $\\tprate=0.95$"
