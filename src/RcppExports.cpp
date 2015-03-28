// This file was generated by Rcpp::compileAttributes
// Generator token: 10BE3573-1514-4C36-9D1C-5A225CD40393

#include <RcppArmadillo.h>
#include <Rcpp.h>

using namespace Rcpp;

// compute_batch_variables
RcppExport SEXP compute_batch_variables(Function kstest, SEXP xmod);
RcppExport SEXP CNPBayes_compute_batch_variables(SEXP kstestSEXP, SEXP xmodSEXP) {
BEGIN_RCPP
    Rcpp::RObject __result;
    Rcpp::RNGScope __rngScope;
    Rcpp::traits::input_parameter< Function >::type kstest(kstestSEXP);
    Rcpp::traits::input_parameter< SEXP >::type xmod(xmodSEXP);
    __result = Rcpp::wrap(compute_batch_variables(kstest, xmod));
    return __result;
END_RCPP
}
// tableZ
IntegerVector tableZ(int K, IntegerVector z);
RcppExport SEXP CNPBayes_tableZ(SEXP KSEXP, SEXP zSEXP) {
BEGIN_RCPP
    Rcpp::RObject __result;
    Rcpp::RNGScope __rngScope;
    Rcpp::traits::input_parameter< int >::type K(KSEXP);
    Rcpp::traits::input_parameter< IntegerVector >::type z(zSEXP);
    __result = Rcpp::wrap(tableZ(K, z));
    return __result;
END_RCPP
}
// getK
int getK(Rcpp::S4 hyperparams);
RcppExport SEXP CNPBayes_getK(SEXP hyperparamsSEXP) {
BEGIN_RCPP
    Rcpp::RObject __result;
    Rcpp::RNGScope __rngScope;
    Rcpp::traits::input_parameter< Rcpp::S4 >::type hyperparams(hyperparamsSEXP);
    __result = Rcpp::wrap(getK(hyperparams));
    return __result;
END_RCPP
}
// tableBatchZ
RcppExport SEXP tableBatchZ(SEXP xmod);
RcppExport SEXP CNPBayes_tableBatchZ(SEXP xmodSEXP) {
BEGIN_RCPP
    Rcpp::RObject __result;
    Rcpp::RNGScope __rngScope;
    Rcpp::traits::input_parameter< SEXP >::type xmod(xmodSEXP);
    __result = Rcpp::wrap(tableBatchZ(xmod));
    return __result;
END_RCPP
}
// compute_loglik_batch
RcppExport SEXP compute_loglik_batch(SEXP xmod);
RcppExport SEXP CNPBayes_compute_loglik_batch(SEXP xmodSEXP) {
BEGIN_RCPP
    Rcpp::RObject __result;
    Rcpp::RNGScope __rngScope;
    Rcpp::traits::input_parameter< SEXP >::type xmod(xmodSEXP);
    __result = Rcpp::wrap(compute_loglik_batch(xmod));
    return __result;
END_RCPP
}
// update_mu_batch
RcppExport SEXP update_mu_batch(SEXP xmod);
RcppExport SEXP CNPBayes_update_mu_batch(SEXP xmodSEXP) {
BEGIN_RCPP
    Rcpp::RObject __result;
    Rcpp::RNGScope __rngScope;
    Rcpp::traits::input_parameter< SEXP >::type xmod(xmodSEXP);
    __result = Rcpp::wrap(update_mu_batch(xmod));
    return __result;
END_RCPP
}
// update_tau2_batch
RcppExport SEXP update_tau2_batch(SEXP xmod);
RcppExport SEXP CNPBayes_update_tau2_batch(SEXP xmodSEXP) {
BEGIN_RCPP
    Rcpp::RObject __result;
    Rcpp::RNGScope __rngScope;
    Rcpp::traits::input_parameter< SEXP >::type xmod(xmodSEXP);
    __result = Rcpp::wrap(update_tau2_batch(xmod));
    return __result;
END_RCPP
}
// update_sigma20_batch
RcppExport SEXP update_sigma20_batch(SEXP xmod);
RcppExport SEXP CNPBayes_update_sigma20_batch(SEXP xmodSEXP) {
BEGIN_RCPP
    Rcpp::RObject __result;
    Rcpp::RNGScope __rngScope;
    Rcpp::traits::input_parameter< SEXP >::type xmod(xmodSEXP);
    __result = Rcpp::wrap(update_sigma20_batch(xmod));
    return __result;
END_RCPP
}
// update_nu0_batch
RcppExport SEXP update_nu0_batch(SEXP xmod);
RcppExport SEXP CNPBayes_update_nu0_batch(SEXP xmodSEXP) {
BEGIN_RCPP
    Rcpp::RObject __result;
    Rcpp::RNGScope __rngScope;
    Rcpp::traits::input_parameter< SEXP >::type xmod(xmodSEXP);
    __result = Rcpp::wrap(update_nu0_batch(xmod));
    return __result;
END_RCPP
}
// update_multinomialPr_batch
RcppExport SEXP update_multinomialPr_batch(SEXP xmod);
RcppExport SEXP CNPBayes_update_multinomialPr_batch(SEXP xmodSEXP) {
BEGIN_RCPP
    Rcpp::RObject __result;
    Rcpp::RNGScope __rngScope;
    Rcpp::traits::input_parameter< SEXP >::type xmod(xmodSEXP);
    __result = Rcpp::wrap(update_multinomialPr_batch(xmod));
    return __result;
END_RCPP
}
// update_p_batch
RcppExport SEXP update_p_batch(SEXP xmod);
RcppExport SEXP CNPBayes_update_p_batch(SEXP xmodSEXP) {
BEGIN_RCPP
    Rcpp::RObject __result;
    Rcpp::RNGScope __rngScope;
    Rcpp::traits::input_parameter< SEXP >::type xmod(xmodSEXP);
    __result = Rcpp::wrap(update_p_batch(xmod));
    return __result;
END_RCPP
}
// update_z_batch
RcppExport SEXP update_z_batch(SEXP xmod);
RcppExport SEXP CNPBayes_update_z_batch(SEXP xmodSEXP) {
BEGIN_RCPP
    Rcpp::RObject __result;
    Rcpp::RNGScope __rngScope;
    Rcpp::traits::input_parameter< SEXP >::type xmod(xmodSEXP);
    __result = Rcpp::wrap(update_z_batch(xmod));
    return __result;
END_RCPP
}
// compute_means_batch
RcppExport SEXP compute_means_batch(SEXP xmod);
RcppExport SEXP CNPBayes_compute_means_batch(SEXP xmodSEXP) {
BEGIN_RCPP
    Rcpp::RObject __result;
    Rcpp::RNGScope __rngScope;
    Rcpp::traits::input_parameter< SEXP >::type xmod(xmodSEXP);
    __result = Rcpp::wrap(compute_means_batch(xmod));
    return __result;
END_RCPP
}
// compute_prec_batch
RcppExport SEXP compute_prec_batch(SEXP xmod);
RcppExport SEXP CNPBayes_compute_prec_batch(SEXP xmodSEXP) {
BEGIN_RCPP
    Rcpp::RObject __result;
    Rcpp::RNGScope __rngScope;
    Rcpp::traits::input_parameter< SEXP >::type xmod(xmodSEXP);
    __result = Rcpp::wrap(compute_prec_batch(xmod));
    return __result;
END_RCPP
}
// compute_vars_batch
RcppExport SEXP compute_vars_batch(SEXP xmod);
RcppExport SEXP CNPBayes_compute_vars_batch(SEXP xmodSEXP) {
BEGIN_RCPP
    Rcpp::RObject __result;
    Rcpp::RNGScope __rngScope;
    Rcpp::traits::input_parameter< SEXP >::type xmod(xmodSEXP);
    __result = Rcpp::wrap(compute_vars_batch(xmod));
    return __result;
END_RCPP
}
// compute_logprior_batch
RcppExport SEXP compute_logprior_batch(SEXP xmod);
RcppExport SEXP CNPBayes_compute_logprior_batch(SEXP xmodSEXP) {
BEGIN_RCPP
    Rcpp::RObject __result;
    Rcpp::RNGScope __rngScope;
    Rcpp::traits::input_parameter< SEXP >::type xmod(xmodSEXP);
    __result = Rcpp::wrap(compute_logprior_batch(xmod));
    return __result;
END_RCPP
}
// update_theta_batch
RcppExport SEXP update_theta_batch(SEXP xmod);
RcppExport SEXP CNPBayes_update_theta_batch(SEXP xmodSEXP) {
BEGIN_RCPP
    Rcpp::RObject __result;
    Rcpp::RNGScope __rngScope;
    Rcpp::traits::input_parameter< SEXP >::type xmod(xmodSEXP);
    __result = Rcpp::wrap(update_theta_batch(xmod));
    return __result;
END_RCPP
}
// update_sigma2_batch
RcppExport SEXP update_sigma2_batch(SEXP xmod);
RcppExport SEXP CNPBayes_update_sigma2_batch(SEXP xmodSEXP) {
BEGIN_RCPP
    Rcpp::RObject __result;
    Rcpp::RNGScope __rngScope;
    Rcpp::traits::input_parameter< SEXP >::type xmod(xmodSEXP);
    __result = Rcpp::wrap(update_sigma2_batch(xmod));
    return __result;
END_RCPP
}
// compute_probz_batch
RcppExport SEXP compute_probz_batch(SEXP xmod);
RcppExport SEXP CNPBayes_compute_probz_batch(SEXP xmodSEXP) {
BEGIN_RCPP
    Rcpp::RObject __result;
    Rcpp::RNGScope __rngScope;
    Rcpp::traits::input_parameter< SEXP >::type xmod(xmodSEXP);
    __result = Rcpp::wrap(compute_probz_batch(xmod));
    return __result;
END_RCPP
}
// mcmc_batch_burnin
RcppExport SEXP mcmc_batch_burnin(SEXP xmod, SEXP mcmcp);
RcppExport SEXP CNPBayes_mcmc_batch_burnin(SEXP xmodSEXP, SEXP mcmcpSEXP) {
BEGIN_RCPP
    Rcpp::RObject __result;
    Rcpp::RNGScope __rngScope;
    Rcpp::traits::input_parameter< SEXP >::type xmod(xmodSEXP);
    Rcpp::traits::input_parameter< SEXP >::type mcmcp(mcmcpSEXP);
    __result = Rcpp::wrap(mcmc_batch_burnin(xmod, mcmcp));
    return __result;
END_RCPP
}
// mcmc_batch
RcppExport SEXP mcmc_batch(SEXP xmod, SEXP mcmcp);
RcppExport SEXP CNPBayes_mcmc_batch(SEXP xmodSEXP, SEXP mcmcpSEXP) {
BEGIN_RCPP
    Rcpp::RObject __result;
    Rcpp::RNGScope __rngScope;
    Rcpp::traits::input_parameter< SEXP >::type xmod(xmodSEXP);
    Rcpp::traits::input_parameter< SEXP >::type mcmcp(mcmcpSEXP);
    __result = Rcpp::wrap(mcmc_batch(xmod, mcmcp));
    return __result;
END_RCPP
}
// loglik
RcppExport SEXP loglik(SEXP xmod);
RcppExport SEXP CNPBayes_loglik(SEXP xmodSEXP) {
BEGIN_RCPP
    Rcpp::RObject __result;
    Rcpp::RNGScope __rngScope;
    Rcpp::traits::input_parameter< SEXP >::type xmod(xmodSEXP);
    __result = Rcpp::wrap(loglik(xmod));
    return __result;
END_RCPP
}
// update_mu
RcppExport SEXP update_mu(SEXP xmod);
RcppExport SEXP CNPBayes_update_mu(SEXP xmodSEXP) {
BEGIN_RCPP
    Rcpp::RObject __result;
    Rcpp::RNGScope __rngScope;
    Rcpp::traits::input_parameter< SEXP >::type xmod(xmodSEXP);
    __result = Rcpp::wrap(update_mu(xmod));
    return __result;
END_RCPP
}
// update_tau2
RcppExport SEXP update_tau2(SEXP xmod);
RcppExport SEXP CNPBayes_update_tau2(SEXP xmodSEXP) {
BEGIN_RCPP
    Rcpp::RObject __result;
    Rcpp::RNGScope __rngScope;
    Rcpp::traits::input_parameter< SEXP >::type xmod(xmodSEXP);
    __result = Rcpp::wrap(update_tau2(xmod));
    return __result;
END_RCPP
}
// update_sigma2_0
RcppExport SEXP update_sigma2_0(SEXP xmod);
RcppExport SEXP CNPBayes_update_sigma2_0(SEXP xmodSEXP) {
BEGIN_RCPP
    Rcpp::RObject __result;
    Rcpp::RNGScope __rngScope;
    Rcpp::traits::input_parameter< SEXP >::type xmod(xmodSEXP);
    __result = Rcpp::wrap(update_sigma2_0(xmod));
    return __result;
END_RCPP
}
// update_nu0
RcppExport SEXP update_nu0(SEXP xmod);
RcppExport SEXP CNPBayes_update_nu0(SEXP xmodSEXP) {
BEGIN_RCPP
    Rcpp::RObject __result;
    Rcpp::RNGScope __rngScope;
    Rcpp::traits::input_parameter< SEXP >::type xmod(xmodSEXP);
    __result = Rcpp::wrap(update_nu0(xmod));
    return __result;
END_RCPP
}
// update_p
RcppExport SEXP update_p(SEXP xmod);
RcppExport SEXP CNPBayes_update_p(SEXP xmodSEXP) {
BEGIN_RCPP
    Rcpp::RObject __result;
    Rcpp::RNGScope __rngScope;
    Rcpp::traits::input_parameter< SEXP >::type xmod(xmodSEXP);
    __result = Rcpp::wrap(update_p(xmod));
    return __result;
END_RCPP
}
// update_multinomialPr
RcppExport SEXP update_multinomialPr(SEXP xmod);
RcppExport SEXP CNPBayes_update_multinomialPr(SEXP xmodSEXP) {
BEGIN_RCPP
    Rcpp::RObject __result;
    Rcpp::RNGScope __rngScope;
    Rcpp::traits::input_parameter< SEXP >::type xmod(xmodSEXP);
    __result = Rcpp::wrap(update_multinomialPr(xmod));
    return __result;
END_RCPP
}
// update_z
RcppExport SEXP update_z(SEXP xmod);
RcppExport SEXP CNPBayes_update_z(SEXP xmodSEXP) {
BEGIN_RCPP
    Rcpp::RObject __result;
    Rcpp::RNGScope __rngScope;
    Rcpp::traits::input_parameter< SEXP >::type xmod(xmodSEXP);
    __result = Rcpp::wrap(update_z(xmod));
    return __result;
END_RCPP
}
// compute_means
RcppExport SEXP compute_means(SEXP xmod);
RcppExport SEXP CNPBayes_compute_means(SEXP xmodSEXP) {
BEGIN_RCPP
    Rcpp::RObject __result;
    Rcpp::RNGScope __rngScope;
    Rcpp::traits::input_parameter< SEXP >::type xmod(xmodSEXP);
    __result = Rcpp::wrap(compute_means(xmod));
    return __result;
END_RCPP
}
// compute_vars
RcppExport SEXP compute_vars(SEXP xmod);
RcppExport SEXP CNPBayes_compute_vars(SEXP xmodSEXP) {
BEGIN_RCPP
    Rcpp::RObject __result;
    Rcpp::RNGScope __rngScope;
    Rcpp::traits::input_parameter< SEXP >::type xmod(xmodSEXP);
    __result = Rcpp::wrap(compute_vars(xmod));
    return __result;
END_RCPP
}
// compute_prec
RcppExport SEXP compute_prec(SEXP xmod);
RcppExport SEXP CNPBayes_compute_prec(SEXP xmodSEXP) {
BEGIN_RCPP
    Rcpp::RObject __result;
    Rcpp::RNGScope __rngScope;
    Rcpp::traits::input_parameter< SEXP >::type xmod(xmodSEXP);
    __result = Rcpp::wrap(compute_prec(xmod));
    return __result;
END_RCPP
}
// compute_logprior
RcppExport SEXP compute_logprior(SEXP xmod);
RcppExport SEXP CNPBayes_compute_logprior(SEXP xmodSEXP) {
BEGIN_RCPP
    Rcpp::RObject __result;
    Rcpp::RNGScope __rngScope;
    Rcpp::traits::input_parameter< SEXP >::type xmod(xmodSEXP);
    __result = Rcpp::wrap(compute_logprior(xmod));
    return __result;
END_RCPP
}
// update_sigma2
RcppExport SEXP update_sigma2(SEXP xmod);
RcppExport SEXP CNPBayes_update_sigma2(SEXP xmodSEXP) {
BEGIN_RCPP
    Rcpp::RObject __result;
    Rcpp::RNGScope __rngScope;
    Rcpp::traits::input_parameter< SEXP >::type xmod(xmodSEXP);
    __result = Rcpp::wrap(update_sigma2(xmod));
    return __result;
END_RCPP
}
// mcmc_marginal_burnin
RcppExport SEXP mcmc_marginal_burnin(SEXP xmod, SEXP mcmcp);
RcppExport SEXP CNPBayes_mcmc_marginal_burnin(SEXP xmodSEXP, SEXP mcmcpSEXP) {
BEGIN_RCPP
    Rcpp::RObject __result;
    Rcpp::RNGScope __rngScope;
    Rcpp::traits::input_parameter< SEXP >::type xmod(xmodSEXP);
    Rcpp::traits::input_parameter< SEXP >::type mcmcp(mcmcpSEXP);
    __result = Rcpp::wrap(mcmc_marginal_burnin(xmod, mcmcp));
    return __result;
END_RCPP
}
// mcmc_marginal
RcppExport SEXP mcmc_marginal(SEXP xmod, SEXP mcmcp);
RcppExport SEXP CNPBayes_mcmc_marginal(SEXP xmodSEXP, SEXP mcmcpSEXP) {
BEGIN_RCPP
    Rcpp::RObject __result;
    Rcpp::RNGScope __rngScope;
    Rcpp::traits::input_parameter< SEXP >::type xmod(xmodSEXP);
    Rcpp::traits::input_parameter< SEXP >::type mcmcp(mcmcpSEXP);
    __result = Rcpp::wrap(mcmc_marginal(xmod, mcmcp));
    return __result;
END_RCPP
}
// marginal_theta
RcppExport SEXP marginal_theta(SEXP xmod, SEXP mcmcp);
RcppExport SEXP CNPBayes_marginal_theta(SEXP xmodSEXP, SEXP mcmcpSEXP) {
BEGIN_RCPP
    Rcpp::RObject __result;
    Rcpp::RNGScope __rngScope;
    Rcpp::traits::input_parameter< SEXP >::type xmod(xmodSEXP);
    Rcpp::traits::input_parameter< SEXP >::type mcmcp(mcmcpSEXP);
    __result = Rcpp::wrap(marginal_theta(xmod, mcmcp));
    return __result;
END_RCPP
}