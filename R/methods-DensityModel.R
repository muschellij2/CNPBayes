setMethod("densitiesCluster", "BatchModel", function(object){
  dens <- densities(object)
  modes <- dens[["modes"]]
  if(length(modes(object)) > 0){
    object <- useModes(object)
  }
  ix <- order(mu(object))
  mus <- mu(object)[ix]
  names(mus) <- ix
  if(length(mus) > length(modes)){
    km <- kmeans(mus, centers=modes)$cluster
    names(km) <- ix
  } else km <- setNames(seq_along(mus), ix)
  nclusters <- length(unique(km))
  batch <- dens[["batch"]]
  batch.list <- split(batch, km)
  batch <- lapply(batch.list, function(x) do.call("+", x))
  component <- dens[["component"]]
  component.list <- split(component, km)
  length(component.list) == nclusters
  component <- lapply(component.list, function(x) do.call("+", x))
  ##component <- lapply(component.list, rowSums)
  overall <- rowSums(do.call(cbind, component))
  modes <- findModes(dens$quantiles, overall) ## should be the same
  list(batch=batch, component=component, overall=overall, modes=modes,
       quantiles=dens$quantiles,
       clusters=km)
})

setMethod("densitiesCluster", "MarginalModel", function(object){
  dens <- densities(object)
  modes <- dens[["modes"]]
  if(length(modes(object)) > 0){
    object <- useModes(object)
  }
  ix <- order(theta(object))
  thetas <- theta(object)[ix]
  names(thetas) <- ix
  if(length(thetas) > length(modes)){
    km <- kmeans(thetas, centers=modes)$cluster
    names(km) <- ix
  } else km <- setNames(seq_along(modes), ix)
  nclusters <- length(unique(km))
  component <- dens[["component"]]
  component.list <- split(component, km)
  length(component.list) == nclusters
  component <- lapply(component.list, function(x) do.call("+", x))
  overall <- rowSums(do.call(cbind, component))
  modes <- findModes(dens$quantiles, overall) ## should be the same
  list(component=component, overall=overall, modes=modes,
       clusters=km, quantiles=dens$quantiles)
})

#' Constructor for DensityModel class
#'
#' Instantiates an instance of 'DensityModel' (or 'DensityBatchModel')
#' from a MarginalModel or BatchModel object. See the corresponding
#' class for additional details and examples.
#' @seealso \code{\link{DensityModel-class}} \code{\link{kmeans}}
#' @param object see \code{showMethods(DensityModel)}
#' @param merge Logical.  Whether to use kmeans clustering to cluster
#' the component means using the estimated modes from the overall
#' density as the centers for the \code{kmeans} function.
#' @export
DensityModel <- function(object, merge=FALSE){
  if(!missing(object)){
    if(!merge){
      dens <- densities(object)
    } else{
      dens <- densitiesCluster(object)
    }
    quantiles <- dens$quantiles
    clusters <- dens$clusters
    component <- dens$component
    overall <- dens$overall
    modes <- dens$modes
  } else{
    ## object not provided
    component <- list()
    overall <- list()
    modes <- numeric()
    clusters <- numeric()
    quantiles <- numeric()
  }
  if(isMarginalModel(object)){
    obj <- new("DensityModel", component=component, overall=overall, modes=modes,
               clusters=clusters, quantiles=quantiles)
    return(obj)
  }
  if(!missing(object)){
    batch <- dens$batch
  } else batch <- list()
  new("DensityBatchModel", batch=batch, component=component, overall=overall,
      modes=modes, clusters=clusters, quantiles=quantiles)
}

setMethod("component", "DensityModel", function(object) object@component)
setMethod("batch", "DensityModel", function(object) object@batch)

setMethod("overall", "DensityModel", function(object) object@overall)
setMethod("modes", "DensityModel", function(object) object@modes)

setMethod("k", "DensityModel", function(object) length(component(object)))

setMethod("show", "DensityModel", function(object){
  cat("An object of class 'DensityModel'\n")
  cat("   component densities:  list of", length(component(object)), "vectors \n")
  cat("   overall density:  vector of length",  length(overall(object)), "\n")
  cat("   modes: ",  paste(sort(round(modes(object), 2)), collapse=", "), "\n")
})

setMethod("show", "DensityBatchModel", function(object){
  cat("An object of class 'DensityBatchModel'\n")
  cat("   batch: list of ", length(component(object)), "matrices \n")
  cat("   component densities:  list of", length(component(object)), "vectors \n")
  cat("   overall density:  vector of length",  length(overall(object)), "\n")
  cat("   modes: ",  paste(sort(round(modes(object), 2)), collapse=", "), "\n")
})


.plotMarginal <- function(x, y, ...){
  object <- x
  args <- list(...)
  if(!"breaks" %in% names(args)){
    L <- length(y)
    hist(y, breaks=L/10,
         col="gray",
         border="gray",  xlab="y",
         freq=FALSE, ...)
  } else {
    hist(y, col="gray", border="gray", xlab="y",
         freq=FALSE, ...)
  }
  comp <- component(x)
  K <- length(comp)
  cols <- brewer.pal(max(K, 3), "Set1")
  cols <- cols[seq_len(K)]
  quantiles <- seq(min(y), max(y),  length.out=250)
  drawdens <- function(y, x, col, lwd=1) lines(x, y, col=col, lwd=lwd)
  mapply(drawdens, y=comp, col=cols, MoreArgs=list(x=quantiles, lwd=2))
  marg <- overall(object)
  lines(quantiles, marg, col="black", lwd=1)
}

.plotBatch <- function(x, y, show.batch=TRUE, ...){
  object <- x
  args <- list(...)
  if(!"breaks" %in% names(args)){
    L <- length(y)
    hist(y, breaks=L/10,
         col="gray",
         border="gray",  xlab="y",
         freq=FALSE, ...)
  } else {
    hist(y, col="gray", border="gray", xlab="y",
         freq=FALSE, ...)
  }
  comp <- component(x)
  K <- length(comp)
  cols <- brewer.pal(max(K, 3), "Set1")
  cols <- cols[seq_len(K)]
  drawdens <- function(y, x, col, lwd=1) lines(x, y, col=col, lwd=lwd)
  if(show.batch){
    batches <- batch(object)
    for(j in seq_along(batches)){
      B <- batches[[j]]
      apply(B, 2, drawdens, col=cols[j], x=quantiles(object), lwd=1)
    }
  }
  mapply(drawdens, y=comp, col=cols, MoreArgs=list(x=quantiles(object), lwd=2))
  marg <- overall(object)
  lines(quantiles, marg, col="black", lwd=1)
}

dens <- function(x, mean, sd, p1, p2){
  p.x <- p1*p2*dnorm(x, mean, sd)
  p.x
}

##drawEachComponent <- function(x, batches, thetas, sds, P, batchPr,
##cols, plot=TRUE){

batchDensities <- function(x, batches, thetas, sds, P, batchPr){
  mlist <- list()
  for(j in seq_len(ncol(thetas))){
    mlist[[j]] <- .batchdens(x, batches, thetas[, j], sds[, j], P[, j], batchPr)
  }
  ##marginal <- do.call(cbind, mlist)
  mlist
}

.batchdens <- function(x, batches, thetas, sds, p1, p2){
  marginal <- matrix(NA, length(x), length(batches))
  for(b in seq_along(batches)){
    marginal[, b] <- dens(x, thetas[b], sds[b], p1[b], p2[b])
  }
  marginal
}

findModes <- function(quantiles, x){ ##quantiles, density
  signs <- sign(diff(x))
  signs <- c(signs[1], signs)
  inflect <- c(0, cumsum(diff(signs) < 0))
  indices <- tapply(seq_along(inflect), inflect, max)
  indices <- indices[-length(indices)]
  modes <- quantiles[indices]
  modes
}


setMethod("plot", "DensityModel", function(x, y, ...){
  .plotMarginal(x, y, ...)
})

setMethod("plot", "MarginalModel", function(x, y, ...){
  object <- DensityModel(x)
  .plotMarginal(object, oned(x), ...)
  return(object)
})

setMethod("plot", "BatchModel", function(x, y, ...){
  object <- DensityModel(x)
  .plotBatch(object, oned(x), ...)
  return(object)
})


setMethod("plot", "DensityBatchModel", function(x, y, show.batch=TRUE, ...){
  .plotBatch(x, y, show.batch, ...)
})


setMethod("hist", "MixtureModel", function(x, ...){
  op <- par(las=1)
  yy <- y(x)
  ##if(rsample > 0) yy <- sample(yy, rsample)
  args <- list(...)
  if(!"breaks" %in% names(args)){
    L <- length(yy)
    hist(yy, breaks=L/10,
         col="gray",
         border="gray",  xlab="y",
         freq=FALSE, ...)
  } else {
    hist(yy, col="gray", border="gray", xlab="y",
         freq=FALSE, ...)
  }
  par(op)
})

setMethod("densities", "BatchModel", function(object){
  quantiles <- seq(min(observed(object)), max(observed(object)),  length.out=250)
  ## use the modes if available
  if(length(modes(object)) > 0){
    object <- useModes(object)
  }
  thetas <- theta(object)
  sds <- sigma(object)
  P <- p(object)
  P <- matrix(P, nBatch(object), k(object), byrow=TRUE)
  rownames(P) <- uniqueBatch(object)
  ix <- order(mu(object))
  thetas <- thetas[, ix, drop=FALSE]
  sds <- sds[, ix, drop=FALSE]
  P <- P[, ix, drop=FALSE]
  batchPr <- table(batch(object))/length(y(object))
  dens.list <- batchDensities(quantiles, uniqueBatch(object),
                              thetas, sds, P, batchPr)
  component <- lapply(dens.list, rowSums)
  overall <- rowSums(do.call(cbind, component))
  modes <- findModes(quantiles, overall)
  clusters <- seq_len(k(object))
  names(clusters) <- ix
  list(batch=dens.list, component=component, overall=overall, modes=modes,
       clusters=clusters, quantiles=quantiles)
})

setMethod("densities", "MarginalModel", function(object){
  quantiles <- seq(min(observed(object)), max(observed(object)),  length.out=250)
  ## use the modes if available
  if(length(modes(object)) > 0){
    object <- useModes(object)
  }
  thetas <- theta(object)
  sds <- sigma(object)
  P <- p(object)
  ix <- order(thetas)
  thetas <- thetas[ix]
  sds <- sds[ix]
  P <- P[ix]
  dens.list <- vector("list", length(thetas))
  for(i in seq_along(dens.list)){
    dens.list[[i]] <- P[i]*dnorm(quantiles, mean=thetas[i], sd=sds[i])
  }
  ##component <- lapply(dens.list, rowSums)
  overall <- rowSums(do.call(cbind, dens.list))
  modes <- findModes(quantiles, overall)
  clusters <- seq_len(k(object))
  names(clusters) <- ix
  list(component=dens.list, overall=overall, modes=modes,
       clusters=clusters, quantiles=quantiles)
})

setMethod("clusters", "DensityModel", function(object) object@clusters)
setMethod("quantiles", "DensityModel", function(object) object@quantiles)