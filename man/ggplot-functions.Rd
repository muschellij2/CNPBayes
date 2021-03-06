% Generated by roxygen2: do not edit by hand
% Please edit documentation in R/plot-functions.R
\docType{methods}
\name{ggSingleBatchChains}
\alias{ggSingleBatchChains}
\alias{ggMultiBatchChains}
\alias{ggSingleBatch}
\alias{ggMultiBatch}
\alias{ggSingleBatch,MarginalModel-method}
\alias{ggSingleBatch,SingleBatchModel-method}
\alias{ggSingleBatch,SingleBatchCopyNumber-method}
\alias{multiBatchDensities}
\alias{ggMultiBatch,BatchModel-method}
\alias{ggMultiBatch,MultiBatchModel-method}
\alias{ggMultiBatch,MultiBatchCopyNumber-method}
\title{ggplot-style functions for diagnosing convergence}
\usage{
ggSingleBatchChains(model)

ggMultiBatchChains(model)

ggSingleBatch(model, bins)

ggMultiBatch(model, bins)

\S4method{ggSingleBatch}{MarginalModel}(model, bins)

\S4method{ggSingleBatch}{SingleBatchModel}(model, bins)

\S4method{ggSingleBatch}{SingleBatchCopyNumber}(model, bins)

multiBatchDensities(model)

\S4method{ggMultiBatch}{BatchModel}(model, bins)

\S4method{ggMultiBatch}{MultiBatchModel}(model, bins)

\S4method{ggMultiBatch}{MultiBatchCopyNumber}(model, bins)
}
\arguments{
\item{model}{a \code{BatchModel} or  \code{MarginalModel} object}

\item{bins}{length-one integer vector indicating the number of bins for the histograms (passed to \code{geom_histogram})}
}
\value{
a list of \code{ggplot} objects. Chains are grouped by the length of
  the parameter vector. For example, in the single-batch model, the means
  (theta) and variances (sigma2) are component-specific (length k, where k is
  number of components) and are plotted together in a single \code{ggplot}
  object.

a \code{ggplot} object
}
\description{
ggplot-style functions for diagnosing convergence

ggplot wrapper for plotting the data at a single CNP and the model-based densities
}
\examples{
plist.sb <- ggSingleBatchChains(MarginalModelExample)
## chains for parameter vectors of length k
plist.sb[["comp"]]
## chains for parameters vectors of length 1
plist.sb[["single"]]
plist.mb <- ggMultiBatchChains(BatchModelExample)
## chains for parameters that are batch- and component-specific
plist.mb[["batch"]]
## chains for parameters vectors of length k
plist.mb[["comp"]]
## chains for parameter vectors of length 1
plist.mb[["single"]]
ggMultiBatch(MultiBatchModelExample)
ggSingleBatch(SingleBatchModelExample)
df <- multiBatchDensities(MultiBatchModelExample)
head(df)
}
