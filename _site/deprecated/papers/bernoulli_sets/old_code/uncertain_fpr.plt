# set terminal pngcairo  transparent enhanced font "arial,10" fontscale 1.0 size 600, 400 
# set output 'prob2.6.png'
set format x "%2.0f" 
set format y "%3.2f" 
set key fixed right top vertical Right noreverse enhanced autotitle box lt black linewidth 1.000 dashtype solid
set label 1 "mu" at 3.83333, 0.0304860, 0.00000 left norotate back nopoint
set label 2 "sigma" at 5.23046, 0.173192, 0.00000 left norotate back nopoint
set arrow 1 from 3.33333, 0.00000, 0.00000 to 3.33333, 0.285546, 0.00000 nohead back nofilled linewidth 1.000 dashtype solid
set arrow 2 from 3.33333, 0.173192, 0.00000 to 4.73046, 0.173192, 0.00000 nohead back nofilled linewidth 1.000 dashtype solid
set style increment default
set samples 200, 200
set style data lines
set ytics  norangelimit 0.00000,0.030486,0.304860
set title "hypergeometric PDF using normal approximation" 
set xlabel "k, x ->" 
set xrange [ -2.00000 : 10.0000 ] noreverse nowriteback
set x2range [ * : * ] noreverse writeback
set ylabel "probability density ->" 
set yrange [ 0.00000 : 0.304860 ] noreverse nowriteback
set y2range [ * : * ] noreverse writeback
set zrange [ * : * ] noreverse writeback
set cbrange [ * : * ] noreverse writeback
set rrange [ * : * ] noreverse writeback
isint(x)=(int(x)==x)
Binv(p,q)=exp(lgamma(p+q)-lgamma(p)-lgamma(q))
arcsin(x,r)=r<=0?1/0:abs(x)>r?0.0:invpi/sqrt(r*r-x*x)
beta(x,p,q)=p<=0||q<=0?1/0:x<0||x>1?0.0:Binv(p,q)*x**(p-1.0)*(1.0-x)**(q-1.0)
binom(x,n,p)=p<0.0||p>1.0||n<0||!isint(n)?1/0:  !isint(x)?1/0:x<0||x>n?0.0:exp(lgamma(n+1)-lgamma(n-x+1)-lgamma(x+1)  +x*log(p)+(n-x)*log(1.0-p))
cauchy(x,a,b)=b<=0?1/0:b/(pi*(b*b+(x-a)**2))
chisq(x,k)=k<=0||!isint(k)?1/0:  x<=0?0.0:exp((0.5*k-1.0)*log(x)-0.5*x-lgamma(0.5*k)-k*0.5*log2)
erlang(x,n,lambda)=n<=0||!isint(n)||lambda<=0?1/0:  x<0?0.0:x==0?(n==1?real(lambda):0.0):exp(n*log(lambda)+(n-1.0)*log(x)-lgamma(n)-lambda*x)
extreme(x,mu,alpha)=alpha<=0?1/0:alpha*(exp(-alpha*(x-mu)-exp(-alpha*(x-mu))))
f(x,d1,d2)=d1<=0||!isint(d1)||d2<=0||!isint(d2)?1/0:  Binv(0.5*d1,0.5*d2)*(real(d1)/d2)**(0.5*d1)*x**(0.5*d1-1.0)/(1.0+(real(d1)/d2)*x)**(0.5*(d1+d2))
gmm(x,rho,lambda)=rho<=0||lambda<=0?1/0:  x<0?0.0:x==0?(rho>1?0.0:rho==1?real(lambda):1/0):  exp(rho*log(lambda)+(rho-1.0)*log(x)-lgamma(rho)-lambda*x)
geometric(x,p)=p<=0||p>1?1/0:  !isint(x)?1/0:x<0||p==1?(x==0?1.0:0.0):exp(log(p)+x*log(1.0-p))
halfnormal(x,sigma)=sigma<=0?1/0:x<0?0.0:sqrt2invpi/sigma*exp(-0.5*(x/sigma)**2)
hypgeo(x,N,C,d)=N<0||!isint(N)||C<0||C>N||!isint(C)||d<0||d>N||!isint(d)?1/0:  !isint(x)?1/0:x>d||x>C||x<0||x<d-(N-C)?0.0:exp(lgamma(C+1)-lgamma(C-x+1)-lgamma(x+1)+  lgamma(N-C+1)-lgamma(d-x+1)-lgamma(N-C-d+x+1)+lgamma(N-d+1)+lgamma(d+1)-lgamma(N+1))
laplace(x,mu,b)=b<=0?1/0:0.5/b*exp(-abs(x-mu)/b)
logistic(x,a,lambda)=lambda<=0?1/0:lambda*exp(-lambda*(x-a))/(1.0+exp(-lambda*(x-a)))**2
lognormal(x,mu,sigma)=sigma<=0?1/0:  x<0?0.0:invsqrt2pi/sigma/x*exp(-0.5*((log(x)-mu)/sigma)**2)
maxwell(x,a)=a<=0?1/0:x<0?0.0:fourinvsqrtpi*a**3*x*x*exp(-a*a*x*x)
negbin(x,r,p)=r<=0||!isint(r)||p<=0||p>1?1/0:  !isint(x)?1/0:x<0?0.0:p==1?(x==0?1.0:0.0):exp(lgamma(r+x)-lgamma(r)-lgamma(x+1)+  r*log(p)+x*log(1.0-p))
nexp(x,lambda)=lambda<=0?1/0:x<0?0.0:lambda*exp(-lambda*x)
normal(x,mu,sigma)=sigma<=0?1/0:invsqrt2pi/sigma*exp(-0.5*((x-mu)/sigma)**2)
pareto(x,a,b)=a<=0||b<0||!isint(b)?1/0:x<a?0:real(b)/x*(real(a)/x)**b
poisson(x,mu)=mu<=0?1/0:!isint(x)?1/0:x<0?0.0:exp(x*log(mu)-lgamma(x+1)-mu)
rayleigh(x,lambda)=lambda<=0?1/0:x<0?0.0:lambda*2.0*x*exp(-lambda*x*x)
sine(x,f,a)=a<=0?1/0:  x<0||x>=a?0.0:f==0?1.0/a:2.0/a*sin(f*pi*x/a)**2/(1-sin(twopi*f))
t(x,nu)=nu<0||!isint(nu)?1/0:  Binv(0.5*nu,0.5)/sqrt(nu)*(1+real(x*x)/nu)**(-0.5*(nu+1.0))
triangular(x,m,g)=g<=0?1/0:x<m-g||x>=m+g?0.0:1.0/g-abs(x-m)/real(g*g)
uniform(x,a,b)=x<(a<b?a:b)||x>=(a>b?a:b)?0.0:1.0/abs(b-a)
weibull(x,a,lambda)=a<=0||lambda<=0?1/0:  x<0?0.0:x==0?(a>1?0.0:a==1?real(lambda):1/0):  exp(log(a)+a*log(lambda)+(a-1)*log(x)-(lambda*x)**a)
carcsin(x,r)=r<=0?1/0:x<-r?0.0:x>r?1.0:0.5+invpi*asin(x/r)
cbeta(x,p,q)=ibeta(p,q,x)
cbinom(x,n,p)=p<0.0||p>1.0||n<0||!isint(n)?1/0:  !isint(x)?1/0:x<0?0.0:x>=n?1.0:ibeta(n-x,x+1.0,1.0-p)
ccauchy(x,a,b)=b<=0?1/0:0.5+invpi*atan((x-a)/b)
cchisq(x,k)=k<=0||!isint(k)?1/0:x<0?0.0:igamma(0.5*k,0.5*x)
cerlang(x,n,lambda)=n<=0||!isint(n)||lambda<=0?1/0:x<0?0.0:igamma(n,lambda*x)
cextreme(x,mu,alpha)=alpha<=0?1/0:exp(-exp(-alpha*(x-mu)))
cf(x,d1,d2)=d1<=0||!isint(d1)||d2<=0||!isint(d2)?1/0:1.0-ibeta(0.5*d2,0.5*d1,d2/(d2+d1*x))
cgmm(x,rho,lambda)=rho<=0||lambda<=0?1/0:x<0?0.0:igamma(rho,x*lambda)
cgeometric(x,p)=p<=0||p>1?1/0:  !isint(x)?1/0:x<0||p==0?0.0:p==1?1.0:1.0-exp((x+1)*log(1.0-p))
chalfnormal(x,sigma)=sigma<=0?1/0:x<0?0.0:erf(x/sigma/sqrt2)
chypgeo(x,N,C,d)=N<0||!isint(N)||C<0||C>N||!isint(C)||d<0||d>N||!isint(d)?1/0:  !isint(x)?1/0:x<0||x<d-(N-C)?0.0:x>d||x>C?1.0:hypgeo(x,N,C,d)+chypgeo(x-1,N,C,d)
claplace(x,mu,b)=b<=0?1/0:(x<mu)?0.5*exp((x-mu)/b):1.0-0.5*exp(-(x-mu)/b)
clogistic(x,a,lambda)=lambda<=0?1/0:1.0/(1+exp(-lambda*(x-a)))
clognormal(x,mu,sigma)=sigma<=0?1/0:x<=0?0.0:cnormal(log(x),mu,sigma)
cnormal(x,mu,sigma)=sigma<=0?1/0:0.5+0.5*erf((x-mu)/sigma/sqrt2)
cmaxwell(x,a)=a<=0?1/0:x<0?0.0:igamma(1.5,a*a*x*x)
cnegbin(x,r,p)=r<=0||!isint(r)||p<=0||p>1?1/0:  !isint(x)?1/0:x<0?0.0:ibeta(r,x+1,p)
cnexp(x,lambda)=lambda<=0?1/0:x<0?0.0:1-exp(-lambda*x)
cpareto(x,a,b)=a<=0||b<0||!isint(b)?1/0:x<a?0.0:1.0-(real(a)/x)**b
cpoisson(x,mu)=mu<=0?1/0:!isint(x)?1/0:x<0?0.0:1-igamma(x+1.0,mu)
crayleigh(x,lambda)=lambda<=0?1/0:x<0?0.0:1.0-exp(-lambda*x*x)
csine(x,f,a)=a<=0?1/0:  x<0?0.0:x>a?1.0:f==0?real(x)/a:(real(x)/a-sin(f*twopi*x/a)/(f*twopi))/(1.0-sin(twopi*f)/(twopi*f))
ct(x,nu)=nu<0||!isint(nu)?1/0:0.5+0.5*sgn(x)*(1-ibeta(0.5*nu,0.5,nu/(nu+x*x)))
ctriangular(x,m,g)=g<=0?1/0:  x<m-g?0.0:x>=m+g?1.0:0.5+real(x-m)/g-(x-m)*abs(x-m)/(2.0*g*g)
cuniform(x,a,b)=x<(a<b?a:b)?0.0:x>=(a>b?a:b)?1.0:real(x-a)/(b-a)
cweibull(x,a,lambda)=a<=0||lambda<=0?1/0:x<0?0.0:1.0-exp(-(lambda*x)**a)
rnd(x) = floor(x+0.5)
fourinvsqrtpi = 2.25675833419103
invpi = 0.318309886183791
invsqrt2pi = 0.398942280401433
log2 = 0.693147180559945
sqrt2 = 1.4142135623731
sqrt2invpi = 0.797884560802865
twopi = 6.28318530717959
r_xmin = -1
r_sigma = 4.0
n = 10
p = 0.333333333333333
mu = 3.33333333333333
sigma = 1.39712274047485
xmin = -1
xmax = 9
ymax = 0.30485951131839
lambda = 0.3
rho = 0.7
nn = 75
mm = 25
plot hypgeo(rnd(x), nn, mm, n) with histeps, normal(x, mu, sigma)