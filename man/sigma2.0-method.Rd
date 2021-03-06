% Generated by roxygen2: do not edit by hand
% Please edit documentation in R/AllGenerics.R, R/methods-McmcChains.R,
%   R/methods-MixtureModel.R
\docType{methods}
\name{sigma2.0}
\alias{sigma2.0}
\alias{sigma2.0,McmcChains-method}
\alias{sigma2.0,MixtureModel-method}
\title{Retrieve the rate parameter for the sigma.2 distribution.}
\usage{
sigma2.0(object)

\S4method{sigma2.0}{McmcChains}(object)

\S4method{sigma2.0}{MixtureModel}(object)
}
\arguments{
\item{object}{see \code{showMethods(sigma2.0)}}
}
\value{
A length 1 numeric
}
\description{
Retrieve the rate parameter for the sigma.2 distribution.
}
\examples{
     sigma2.0(MarginalModelExample)
}
