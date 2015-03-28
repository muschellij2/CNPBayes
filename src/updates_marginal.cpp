#include "update.h" // getK
#include "miscfunctions.h" // for rdirichlet, tableZ, ...
#include <Rmath.h>
#include <Rcpp.h>

using namespace Rcpp ;

// [[Rcpp::export]]
RcppExport SEXP loglik(SEXP xmod) {
    RNGScope scope ;
    Rcpp::S4 model(xmod) ;
    NumericVector x = model.slot("data") ;
    NumericVector p = model.slot("pi") ;
    int K = getK(model.slot("hyperparams")) ;
    NumericVector theta = model.slot("theta") ;
    NumericVector sigma2 = model.slot("sigma2") ;
    NumericVector sigma = sqrt(sigma2) ;
    int n = x.size() ;
    //double lik;
    NumericVector loglik(1) ;
    NumericVector y(1);    
    NumericVector lik(n);
    // Below is equivalent to rowSums(lik) in .loglikMarginal
    for(int k = 0; k < K; k++) {
      lik += p[k]*dnorm(x, theta[k], sigma[k]);
    }
    for(int i = 0; i < n; i++){
      loglik[0] += log(lik[i]);
    }
    return loglik;
}

//
// This function does not reproduce the R update .updateMu when the
// same seed is used...
//


// [[Rcpp::export]]
RcppExport SEXP update_mu(SEXP xmod) {
  RNGScope scope ;
  Rcpp::S4 model(xmod) ;  
  Rcpp::S4 hypp(model.slot("hyperparams")) ;
  double tau2_0 = hypp.slot("tau2.0") ;
  double tau20_tilde = 1.0/tau2_0 ;
  double tau2 = model.slot("tau2") ;
  double tau2_tilde = 1.0/tau2 ;
  double mu_0 = hypp.slot("mu.0") ;
  double K = getK(hypp) ;
  NumericVector theta = model.slot("theta") ;
  //NumericVector nn = model.slot("zfreq") ;
  IntegerVector z = model.slot("z") ;
  IntegerVector nn = tableZ(K, z) ;
  double thetabar ;
  double total ;
  for(int k = 0; k < K; k++) total += nn[k] ;
  for(int k = 0; k < K; k++) thetabar += nn[k] * theta[k] / total ;
  double mu_K ;
  double post_prec = 1.0/tau2_0 + K*tau2_tilde ;
  double w1 ;
  double w2 ;
  w1 = tau20_tilde/post_prec ;
  w2 = K*tau2_tilde/post_prec ;
  mu_K =  w1*mu_0 +  w2*thetabar ;
  NumericVector mu_new(1);
  double tau_k = sqrt(1.0/post_prec) ;
  mu_new[0] = as<double>(rnorm(1, mu_K, tau_k)) ;
  return mu_new ;
  //return mu_K ;
}

// [[Rcpp::export]]
RcppExport SEXP update_tau2(SEXP xmod) {
  RNGScope scope ;
  Rcpp::S4 model(xmod) ;  
  Rcpp::S4 hypp(model.slot("hyperparams")) ;
  double m2_0 = hypp.slot("m2.0") ;
  int K = getK(hypp) ;
  double eta_0 = hypp.slot("eta.0") ;
  // should eta_k be double or integer?
  double eta_k = eta_0 + K ;
  double mu = model.slot("mu") ;
  NumericVector theta = model.slot("theta") ;
  NumericVector s2_k(1) ;
  for(int k = 0; k < K; k++) s2_k[0] += (theta[k] - mu) * (theta[k] - mu) ;
  NumericVector m2_k(1) ;
  m2_k[0] = 1/eta_k*(eta_0*m2_0 + s2_k[0]) ;

  NumericVector tau2(1) ;
  //  rgamma is parameterized by scale.  In R, I've parameterized by rate
  tau2[0] = 1/as<double>(rgamma(1, 0.5*eta_k, 1.0/(0.5*eta_k*m2_k[0]))) ;
  //   tau2[0] = 1/as<double>(rgamma(1, 1/2*eta_k, (1/2*eta_k*m2_k[0]))) ;
  // In R, I check that this is valid and simulate from the prior if not
  return tau2 ;
}

// [[Rcpp::export]]
RcppExport SEXP update_sigma2_0(SEXP xmod) {
  RNGScope scope ;
  Rcpp::S4 model(xmod) ;  
  Rcpp::S4 hypp(model.slot("hyperparams")) ;
  double a = hypp.slot("a") ;
  double b = hypp.slot("b") ;
  double nu_0 = model.slot("nu.0") ;
  NumericVector sigma2 = model.slot("sigma2") ;
  //  return sigma2 ;
  int K = getK(hypp) ;
  double a_k = a + 0.5*K*nu_0 ;
  //NumericVector b_k(1) ;
  double b_k ;
  for(int k=0; k < K; k++) b_k += 0.5*1.0/sigma2[k] ;
  b_k += b ;
  //  return b_k ;
  NumericVector sigma2_0(1);
  sigma2_0[0] = as<double>(rgamma(1, a_k, 1.0/b_k)) ;
  return sigma2_0 ;
}

// [[Rcpp::export]]
RcppExport SEXP update_nu0(SEXP xmod) {
  RNGScope scope ;
  Rcpp::S4 model(xmod) ;  
  Rcpp::S4 hypp(model.slot("hyperparams")) ;
  int K = getK(hypp) ;
  double sigma2_0 = model.slot("sigma2.0") ;
  NumericVector sigma2 = model.slot("sigma2") ;
  double betas = hypp.slot("beta") ;
  //
  // sample nu0 from an unnormalized probability distribution
  //
  NumericVector x(100) ;
  NumericVector lpnu0(100);
  double prec = 0.0 ;
  double lprec = 0.0 ;
  for(int k = 0; k < K; k++) prec += 1.0/sigma2[k] ;
  for(int k = 0; k < K; k++) lprec += log(1.0/sigma2[k]) ;
  for(int i = 0; i < 100; i++){
    x[i] = i+1 ;
  }
  NumericVector y1(100) ;
  NumericVector y2(100) ;
  NumericVector y3(100) ;
  y1 = K*(0.5*x*log(sigma2_0*0.5*x) - lgamma(x*0.5)) ;
  y2 = (0.5*x - 1.0) * lprec ;
  y3 = x*(betas + 0.5*sigma2_0*prec) ;
  lpnu0 =  y1 + y2 - y3 ;
  NumericVector prob(100) ;
  prob = exp(lpnu0) ; // - maxprob) ;
  prob = prob/sum(prob) ;
  //double maxprob = max(lpnu0) ;
  NumericVector nu0(1) ;
  //int u ;
  NumericVector u(1) ;
  double cumprob = 0.0 ;
  // sample x with probability prob
  for(int i = 0; i < 100; i++){
    cumprob += prob[i] ;
    u = runif(1) ;
    if (u[0] < cumprob){
      nu0[0] = x[i] ;
      break ;
    }
  }
  return nu0 ;
  //nu0 = max(1, nu0)  // is this needed??
}

// [[Rcpp::export]]
RcppExport SEXP update_p(SEXP xmod) {
  RNGScope scope ;
  Rcpp::S4 model(xmod) ;  
  Rcpp::S4 hypp(model.slot("hyperparams")) ;
  int K = getK(hypp) ;
  // IntegerVector nn = model.slot("zfreq");
  IntegerVector z = model.slot("z") ;
  IntegerVector nn = tableZ(K, z) ;  
  IntegerVector alpha = hypp.slot("alpha") ;
  NumericVector alpha_n(K) ;  // really an integer vector, but rdirichlet expects numeric
  for(int k=0; k < K; k++) alpha_n[k] = alpha[k] + nn[k] ;
  NumericVector p(K) ;
  // pass by reference
  rdirichlet(alpha_n, p) ;
  return p ;
}

// [[Rcpp::export]]
RcppExport SEXP update_multinomialPr(SEXP xmod) {
  RNGScope scope ;
  Rcpp::S4 model(xmod) ;  
  Rcpp::S4 hypp(model.slot("hyperparams")) ;
  int K = getK(hypp) ;
  NumericVector theta = model.slot("theta") ;
  NumericVector sigma2 = model.slot("sigma2") ;
  NumericVector sigma = sqrt(sigma2) ;
  NumericVector p = model.slot("pi") ;
  NumericVector x = model.slot("data") ;
  int n = x.size() ;  
  NumericMatrix lik(n, K) ;
  NumericMatrix probs(n, K) ;
  NumericVector tmp(n) ;
  NumericVector total(n) ;
  for(int k = 0; k < K; k++) {
    tmp = p[k]*dnorm(x, theta[k], sigma[k]) ;
    for(int i = 0; i < n; i++){
      lik(i, k) = tmp[i] ;
    }
    total += tmp ;
  }
  for(int k = 0; k < K; k++){
    for(int i = 0; i < n; i++){
      probs(i, k) = lik(i,k) / total[i] ;
    }
  }
  return probs ;
}

// [[Rcpp::export]]
RcppExport SEXP update_z(SEXP xmod) {
  RNGScope scope ;
  Rcpp::S4 model(xmod) ;  
  Rcpp::S4 hypp(model.slot("hyperparams")) ;
  int K = getK(hypp) ;
  NumericVector x = model.slot("data") ;
  int n = x.size() ;
  NumericMatrix p(n, K);
  p = update_multinomialPr(xmod) ;
  NumericMatrix cumP(n, K);
  for(int i=0; i < n; i++){
    for(int k = 0; k < K; k++){
      if(k > 0){
        cumP(i, k) = cumP(i, k-1) + p(i, k) ;
      } else {
        cumP(i, k) = p(i, k) ;
      }
    }
  }
  //return cumP ;
  NumericVector u = runif(n) ;
  IntegerVector zz(n) ;
  IntegerVector freq(K) ;
  for(int i = 0; i < n; i++){
    int k = 0 ;
    while(k < K) {
      if( u[i] < cumP(i, k)){
        zz[i] = k + 1 ;
        freq[k] += 1 ;
        break ;
      }
      k += 1 ;
    }
    cumP(i, K-1) = 1.00001 ;  // just to be certain
  }
  // To prevent 0 frequencies, arbitrarily switch the label
  if(is_true(any(freq == 0))){
    for(int k = 0; k < K; ++k){
      NumericVector r(1) ;
      r[0] = as<double>(runif(1, 0, 1)) * n ;
      r = round(r, 0) ;
      zz[r[0]] = k + 1 ;
    }
  }
  return zz ;
}

// [[Rcpp::export]]
RcppExport SEXP compute_means(SEXP xmod) {
  RNGScope scope ;
  Rcpp::S4 model(xmod) ;
  NumericVector x = model.slot("data") ;
  int n = x.size() ;
  IntegerVector z = model.slot("z") ;
  Rcpp::S4 hypp(model.slot("hyperparams")) ;
  int K = getK(hypp) ;  
  // IntegerVector nn = model.slot("zfreq") ;
  IntegerVector nn = tableZ(K, z) ;
  NumericVector means( K ) ;
  for(int i = 0; i < n; i++){
    for(int k = 0; k < K; k++){
      if(z[i] == k+1){
        means[k] += x[i] ;
      }
    }
  }
  for(int k = 0; k < K; k++){
    means[k] /= nn[k] ;
  }
  return means ;
}

// [[Rcpp::export]]
RcppExport SEXP compute_vars(SEXP xmod) {
  RNGScope scope ;
  Rcpp::S4 model(xmod) ;
  NumericVector x = model.slot("data") ;
  int n = x.size() ;
  IntegerVector z = model.slot("z") ;
  Rcpp::S4 hypp(model.slot("hyperparams")) ;
  int K = getK(hypp) ;  
  // IntegerVector nn = model.slot("zfreq") ;
  IntegerVector nn ;
  nn = tableZ(K, z) ;
  NumericVector mn = model.slot("theta") ;
  //NumericVector mn = model.slot("data.mean") ;
  NumericVector vars(K) ;
  for(int i = 0; i < n; i++){
    for(int k = 0; k < K; k++){
      if(z[i] == k+1){
        vars[k] += pow(x[i] - mn[k], 2) ;
      }
    }
  }
  for(int k = 0; k < K; k++){
    vars[k] /= nn[k] ;
  }
  return vars ;
}

// [[Rcpp::export]]
RcppExport SEXP compute_prec(SEXP xmod) {
  RNGScope scope ;
  Rcpp::S4 model(xmod) ;
  Rcpp::S4 hypp(model.slot("hyperparams")) ;
  int K = getK(hypp) ;  
  NumericVector vars(K) ;
  NumericVector prec(K) ;
  vars = compute_vars(xmod) ;
  for(int k = 0; k < K; ++k){
    prec[k] = 1.0/vars[k] ;
  }
  return prec ;
}

// [[Rcpp::export]]
RcppExport SEXP compute_logprior(SEXP xmod) {
  RNGScope scope ;
  Rcpp::S4 model(xmod) ;
  Rcpp::S4 hypp(model.slot("hyperparams")) ;
  int K = getK(hypp) ;
  NumericVector mu = model.slot("mu") ;
  NumericVector sigma2_0 = model.slot("sigma2.0") ;
  double a = hypp.slot("a") ;
  double b = hypp.slot("b") ;
  NumericVector nu_0 = model.slot("nu.0") ;
  double mu_0 = hypp.slot("mu.0") ;
  double tau2_0 = hypp.slot("tau2.0") ;
  double tau_0 = sqrt(tau2_0) ;
  double betas = hypp.slot("beta") ;
  NumericVector p_sigma2_0(1) ;
  NumericVector p_mu(1) ;
  NumericVector p_nu_0(1) ;  
  p_sigma2_0 = dgamma(sigma2_0, a, 1/b) ;
  p_nu_0 = dgeom(nu_0, betas) ;
  p_mu = dnorm(mu, mu_0, tau_0) ;
  NumericVector prior_prob(1) ;
  prior_prob = log(p_sigma2_0) + log(p_nu_0) + log(p_mu) ;
  return prior_prob ;
}

//  SEXP compute_llxprior(SEXP xmod) {
//   RNGScope scope ;
//   NumericVector ll(1);
//   NumericVector lprior(1);
//   NumericVector result(1);
//   ll = loglik(xmod) ;
//   lprior = compute_priorPr(xmod) ;
//   result = ll + lprior ;
//   return result ;
// }

// [[Rcpp::export]]
RcppExport SEXP update_sigma2(SEXP xmod){
  RNGScope scope ;
  Rcpp::S4 model(xmod) ;
  Rcpp::S4 hypp(model.slot("hyperparams")) ;
  int K = getK(hypp) ;
  NumericVector x = model.slot("data") ;
  NumericVector theta = model.slot("theta") ;
  double nu_0 = model.slot("nu.0") ;
  double sigma2_0 = model.slot("sigma2.0") ;
  int n = x.size() ;
  IntegerVector z = model.slot("z") ;
  NumericVector nu_n(K) ;
  IntegerVector nn = model.slot("zfreq") ;

  for(int k = 0; k < K; ++k){
    nu_n[k] = nu_0 + nn[k] ;
  }
  
  //  return nn ;
  NumericVector ss(K) ;
  for(int i = 0; i < n; i++){
    int k = 0 ;
    while(k <= K) {
      if( z[i] == k + 1 ){
        ss[k] += pow(x[i] - theta[k], 2.0) ;
        break ;
      }
      k++ ;
    }
  }
  double sigma2_n ;
  NumericVector sigma2_new(K) ;
  for (int k = 0; k < K; k++){
    sigma2_n = 1.0/nu_n[k]*(nu_0*sigma2_0 + ss[k]) ;
    sigma2_new[k] = 1.0/as<double>(rgamma(1, 0.5*nu_n[k], 1.0/(0.5*nu_n[k]*sigma2_n))) ;
  }
  return sigma2_new ;
}


//
// BURNIN: Don't move chains, no thinning, no need to compute loglik at each iteration
//
// [[Rcpp::export]]
RcppExport SEXP mcmc_marginal_burnin(SEXP xmod, SEXP mcmcp) {
  RNGScope scope ;
  Rcpp::S4 model(xmod) ;
  Rcpp::S4 hypp(model.slot("hyperparams")) ;
  int K = getK(hypp) ;  
  Rcpp::S4 params(mcmcp) ;
  IntegerVector up = params.slot("param_updates") ;
  int S = params.slot("burnin") ;
  if( S == 0 ){
    return xmod ;
  }
  for(int s = 0; s < S; ++s){
    if(up[0] > 0)
      model.slot("theta") = update_theta(xmod) ;
    if(up[1] > 0)
      model.slot("sigma2") = update_sigma2(xmod) ;
    if(up[2] > 0)
      model.slot("pi") = update_p(xmod) ;
    if(up[3] > 0)
      model.slot("mu") = update_mu(xmod) ;
    if(up[4] > 0)    
      model.slot("tau2") = update_tau2(xmod) ;
    if(up[5] > 0)    
      model.slot("nu.0") = update_nu0(xmod) ;
    if(up[6] > 0)        
      model.slot("sigma2.0") = update_sigma2_0(xmod) ;
    if(up[7] > 0){        
      model.slot("z") = update_z(xmod) ;
      model.slot("zfreq") = tableZ(K, model.slot("z")) ;
    }
    model.slot("data.mean") = compute_means(xmod) ;
    model.slot("data.prec") = compute_prec(xmod) ;
  }
  // compute log prior probability from last iteration of burnin
  // compute log likelihood from last iteration of burnin
  model.slot("loglik") = loglik(xmod) ;
  model.slot("logprior") = compute_logprior(xmod) ;    
  return xmod ;
}


// Function has gotten pretty long. Might be useful to have a separate
// function whose sole job is to move the chains.  
//
// [[Rcpp::export]]
RcppExport SEXP mcmc_marginal(SEXP xmod, SEXP mcmcp) {
  RNGScope scope ;
  Rcpp::S4 model(xmod) ;
  Rcpp::S4 chain(model.slot("mcmc.chains")) ;
  Rcpp::S4 hypp(model.slot("hyperparams")) ;
  Rcpp::S4 params(mcmcp) ;
  IntegerVector up = params.slot("param_updates") ;
  int K = getK(hypp) ;
  int T = params.slot("thin") ;
  int S = params.slot("iter") ;
  NumericVector x = model.slot("data") ;
  int N = x.size() ;
  NumericMatrix theta = chain.slot("theta") ;
  NumericMatrix sigma2 = chain.slot("sigma2") ;
  NumericMatrix pmix = chain.slot("pi") ;
  NumericMatrix zfreq = chain.slot("zfreq") ;
  NumericVector mu = chain.slot("mu") ;  
  NumericVector tau2 = chain.slot("tau2") ;
  NumericVector nu0 = chain.slot("nu.0") ;
  NumericVector sigma2_0 = chain.slot("sigma2.0") ;
  NumericVector loglik_ = chain.slot("loglik") ;
  NumericVector logprior_ = chain.slot("logprior") ;
  NumericVector th(K) ;
  NumericVector s2(K) ;
  NumericVector p(K) ;
  NumericVector m(1) ; //mu
  NumericVector t2(1) ;//tau2
  NumericVector n0(1) ;//nu0
  IntegerVector z(N) ;
  NumericVector s20(1) ; //sigma2_0
  NumericVector mns(1) ;   
  NumericVector precs(1) ;
  NumericVector ll(1) ;
  NumericVector lp(1) ;
  IntegerVector tmp(K) ;
  IntegerVector zf(K) ;
  // Initial values
  th = model.slot("theta") ;
  s2 = model.slot("sigma2") ;
  p = model.slot("pi") ;
  m = model.slot("mu") ;
  t2 = model.slot("tau2") ;
  n0 = model.slot("nu.0") ;
  s20 = model.slot("sigma2.0") ;
  zf = model.slot("zfreq") ;
  ll = model.slot("loglik") ;
  lp = model.slot("logprior") ;
  // Record initial values in chains
  mu[0] = m[0] ;
  tau2[0] = t2[0] ;
  nu0[0] = n0[0] ;
  sigma2_0[0] = s20[0] ;
  loglik_[0] = ll[0] ;
  logprior_[0] = lp[0] ;
  theta(0, _) = th ;
  sigma2(0, _) = s2 ;
  pmix(0, _) = p ;
  zfreq(0, _) = zf ;
//   for(int k = 0; k < K; k++){  // need update 'xmod' after each update
//     theta(0, k) = th[k] ;
//     sigma2(0, k) = s2[k] ;
//     pmix(0, k) = p[k] ;
//     zfreq(0, k) = zf[k] ;
//   }
  // start at 1 instead of zero. Initial values are as above
  for(int s = 1; s < S; ++s){
    if(up[0] > 0) {
      th = update_theta(xmod) ;
      model.slot("theta") = th ;      
    } else {
      th = model.slot("theta") ;
    }
    theta(s, _) = th ;
    if(up[1] > 0){
      s2 = update_sigma2(xmod) ;
      model.slot("sigma2") = s2 ;
    } else { s2= model.slot("sigma2") ; }
    sigma2(s, _) = s2 ;
    if(up[2] > 0){
      p = update_p(xmod) ;
      model.slot("pi") = p ;
    } else {
      p = model.slot("pi") ;
    }
    pmix(s, _) = p ;
    if(up[3] > 0){
      m = update_mu(xmod) ;
      model.slot("mu") = m ;
    } else {
      m = model.slot("mu") ;
    }
    mu[s] = m[0] ;
    if(up[4] > 0){    
      t2 = update_tau2(xmod) ;
      model.slot("tau2") = t2 ;
    } else {
      t2 = model.slot("tau2") ;
    }
    tau2[s] = t2[0] ;
    if(up[5] > 0){        
      n0 = update_nu0(xmod) ;
      model.slot("nu.0") = n0 ;
    } else {
      n0 = model.slot("nu.0") ;
    }
    nu0[s] = n0[0] ;
    if(up[6] > 0){        
      s20 = update_sigma2_0(xmod) ;
      model.slot("sigma2.0") = s20 ;
    } else {
      s20 = model.slot("sigma2.0") ;
    }
    sigma2_0[s] = s20[0] ;
    if(up[7] > 0){
      z = update_z(xmod) ;
      model.slot("z") = z ;
      tmp = tableZ(K, z) ;
      model.slot("zfreq") = tmp ;
    } else {
      z = model.slot("z") ;
      tmp = model.slot("zfreq") ;
    }
    zfreq(s, _) = tmp ;
    model.slot("data.mean") = compute_means(xmod) ;
    model.slot("data.prec") = compute_prec(xmod) ;
    ll = loglik(xmod) ;
    loglik_[s] = ll[0] ;
    model.slot("loglik") = ll ;
    lp = compute_logprior(xmod) ;
    logprior_[s] = lp[0] ;
    model.slot("logprior") = lp ;
    // Thinning
    for(int t = 0; t < T; ++t){
      if(up[0] > 0)
        model.slot("theta") = update_theta(xmod) ;
      if(up[1] > 0)      
        model.slot("sigma2") = update_sigma2(xmod) ;
      if(up[2] > 0)
        model.slot("pi") = update_p(xmod) ;
      if(up[3] > 0)      
        model.slot("mu") = update_mu(xmod) ;
      if(up[4] > 0)      
        model.slot("tau2") = update_tau2(xmod) ;
      if(up[5] > 0)
        model.slot("nu.0") = update_nu0(xmod) ;
      if(up[6] > 0)
        model.slot("sigma2.0") = update_sigma2_0(xmod) ;
      if(up[7] > 0){
        model.slot("z") = update_z(xmod) ;
        model.slot("zfreq") = tableZ(K, model.slot("z")) ;
      } 
      model.slot("data.mean") = compute_means(xmod) ;
      model.slot("data.prec") = compute_prec(xmod) ;
    }
  }
  //
  // assign chains back to object
  //
  chain.slot("theta") = theta ;
  chain.slot("sigma2") = sigma2 ;
  chain.slot("pi") = pmix ;
  chain.slot("mu") = mu ;
  chain.slot("tau2") = tau2 ;
  chain.slot("nu.0") = nu0 ;
  chain.slot("sigma2.0") = sigma2_0 ;
  chain.slot("zfreq") = zfreq ;
  chain.slot("loglik") = loglik_ ;
  chain.slot("logprior") = logprior_ ;
  model.slot("mcmc.chains") = chain ;
  return xmod ;
}


//
// Estimating marginals
//

// [[Rcpp::export]]
RcppExport SEXP marginal_theta(SEXP xmod, SEXP mcmcp) {
  RNGScope scope ;
  Rcpp::S4 model(xmod) ;
  Rcpp::S4 params(mcmcp) ;
  int S = params.slot("iter") ;
  List modes = model.slot("modes") ;
  NumericVector thetastar = as<NumericVector>(modes["theta"]) ;
  int K = thetastar.size() ;
  NumericVector p_theta(S) ;
  //
  // Run the full Gibbs
  //
  xmod = mcmc_marginal_burnin(xmod, mcmcp) ;
  xmod = mcmc_marginal(xmod, mcmcp) ;
  Rcpp::S4 chains(model.slot("mcmc.chains")) ;
  NumericVector muc = chains.slot("mu") ;
  NumericVector tau2c = chains.slot("tau2") ;
  NumericVector tauc = sqrt(tau2c) ;
  NumericVector tmp(K) ;
  //
  // Compute p(theta* | mu[s], tau[s], ...)
  //   - integrate out mu and tau
  //
  // dnorm works when mu and tau2 are double, but not when they are NumericVectors
  //for(int s = 0; s < S+1; ++s){
  for(int s=0; s < S; ++s){
    tmp = dnorm(thetastar, muc[s], tauc[s]) ;
    double prod = 1.0;
    for(int k = 0; k < K; ++k) {
      prod += log(tmp[k]) ;
    }
    p_theta[s] = prod ;
  }
  return p_theta ;
}

RcppExport SEXP marginal_sigma2(SEXP xmod, SEXP mcmcp) {
  RNGScope scope ;
  Rcpp::S4 model(xmod) ;
  Rcpp::S4 params(mcmcp) ;
  int S = params.slot("iter") ;
  List modes = model.slot("modes") ;
  NumericVector sigma2star = as<NumericVector>(modes["sigma2"]) ;
  NumericVector prec = pow(sigma2star, -1.0) ;
  int K = prec.size() ;
  NumericVector p_prec(S) ;
  //
  // Run the reduced Gibbs
  //
  xmod = mcmc_marginal_burnin(xmod, mcmcp) ;
  xmod = mcmc_marginal(xmod, mcmcp) ;
  //
  Rcpp::S4 chains(model.slot("mcmc.chains")) ;
  NumericVector tmp(K) ;
  NumericVector nu0 = chains.slot("nu.0") ;
  NumericVector s20 = chains.slot("sigma2.0") ;
  for(int s=0; s < S; ++s){
    tmp = dgamma(prec, 0.5*nu0[s], 2.0 / (nu0[s]*s20[s])) ;
    double total = 0.0 ;
    for(int k = 0; k < K; ++k){
      total += log(tmp[k]) ;
    }
    p_prec[s] = total ;
  }
  return p_prec ;
}





