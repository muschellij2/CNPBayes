context("SingleBatchModel")

.test_that <- function(name, expr){}

.test_that("test_constraint", {
    baf <- readRDS(system.file("extdata", "baf.rds", package = "CNPBayes"))
    set.seed(17)
    model <- SingleBatchModel(baf, k = 2)
    model <- posteriorSimulation(model)
})

test_that("test_marginal_empty_component", {
    set.seed(1)
    truth <- simulateData(N = 10, p = rep(1/3, 3), theta = c(-1,
        0, 1), sds = rep(0.1, 3))
    mp <- McmcParams(iter = 5, burnin = 5, nStarts = 1)
    model <- SingleBatchModel(data = y(truth), k = 3, mcmc.params = mp)
    expect_false(any(is.na(CNPBayes:::computeMeans(model))))
})


.test_that("SingleBatchModel2", {
  set.seed(1)
  truth <- simulateData(N = 200,
                        theta = c(-2, -0.4, 0),
                        sds = c(0.3, 0.15, 0.15),
                        p = c(0.005, 1/10, 1 - 0.005 - 1/10))
  yy <- y(truth)
  s <- (yy - median(yy))/sd(yy)
  mp <- McmcParams(iter = 1000, burnin = 1000, nStarts = 1)
  x <- qInverseTau2(mn=0.5, sd=0.5)
  hp <- Hyperparameters(k=3,
                        tau2.0=0.5,
                        mu.0=0,
                        eta.0=x$eta.0,
                        m2.0=x$m2.0)
  hist(sqrt(1/rgamma(1000, 1/2*eta.0(hp), 1/2*eta.0(hp) * m2.0(hp))), breaks=250)

  summary(sqrt(1/rgamma(200, 1/2*eta.0(hp), 1/2*eta.0(hp) * m2.0(hp))))
  ##mns <- rnorm(3, 0, sqrt(1/rgamma(1, 1/2*eta.0(hp), 1/2*eta.0(hp) * m2.0(hp))))
  set.seed(123)
  m <- SingleBatchModel2(data = y(truth),
                      k = 3,
                      mcmc.params = mp,
                      hypp=hp)
  m2 <- posteriorSimulation(m)
  if(FALSE){
    ggSingleBatch(m2)
    plist <- ggSingleBatchChains(m2)
    plist[[1]]
  }
  ##
  ## takes too long
  ##
  library(purrr)
  ##mp <- McmcParams(iter = 1000, burnin = 10, nStarts = 1, thin=1)
  mp <- McmcParams(iter = 1000, burnin = 1000, nStarts = 10, thin=1)
  mod.list <- replicate(4, SingleBatchModel2(data=y(truth), k=3,
                                          mcmc.params=mp, hypp=hp))
  mod.list2 <- map(mod.list, posteriorSimulation)
  mc.list <- mcmcList(mod.list2)
  expect_is(mc.list, "mcmc.list")
  diagnostics(mod.list2)
  model <- combineModels(mod.list2)
  diagnostics(list(model))
  ggSingleBatchChains(model)[[1]]
  ggSingleBatchChains(model)[[2]]
  ggSingleBatch(model)
})

.test_that("segfault", {
    set.seed(1337)
    truth <- simulateData(N = 1000,
                          theta = c(-2, -0.4, 0),
                          sds = c(0.3, 0.15, 0.15),
                          p = c(0.005, 1/10, 1 - 0.005 - 1/10))
    library(purrr)
    mp <- McmcParams(iter = 1000,
                     burnin = 1000,
                     nStarts = 4,
                     thin=10)
    hp <- Hyperparameters(tau2.0=0.4,
                          mu.0=-0.75,
                          eta.0=32,
                          m2.0=0.5)
    set.seed(123)
    mod.list <- gibbs_multipleK(hp=hp, mp=mp, dat=y(truth))

    hp <- Hyperparameters(k=4,
                          tau2.0=0.4,
                          mu.0=-0.75,
                          eta.0=32,
                          m2.0=0.5)
    set.seed(5986)
    model <- gibbs(mp=mp, hp=hp, dat=y(truth))
    set.seed(134)
    model <- gibbs(mp=mp, hp=hp, dat=y(truth))
    set.seed(2496)
    model <- gibbs(mp=mp, hp=hp, dat=y(truth))
    set.seed(496)
    model <- gibbs(mp=mp, hp=hp, dat=y(truth))
    set.seed(1)
    replicate(20, model <- gibbs(mp=mp, hp=hp, dat=y(truth)))

    set.seed(1)
    k(hp) <- 5
    model <- gibbs(mp=mp, hp=hp, dat=y(truth))
})


test_that("test_marginal_few_data", {
  expect_error(model <- SingleBatchModel(data = 0:1, k = 3))
})

test_that("marginal-hard", {
    set.seed(1337)
    truth <- simulateData(N = 1000,
                          theta = c(-2, -0.4, 0),
                          sds = c(0.3, 0.15, 0.15),
                          p = c(0.005, 1/10, 1 - 0.005 - 1/10))
    ## mcmcp <- McmcParams(iter = 1000, burnin = 500, thin = 10,
    ##                     nStarts = 20)
    ##
    ## do enough iterations so that any label switching occurs
    ##
    mcmcp <- McmcParams(iter = 500, burnin = 200, thin = 0,
                        nStarts = 20)
    model <- SingleBatchModel(y(truth), k = 3)
    model <- posteriorSimulation(model)
    i <- order(theta(model))
    expect_identical(i, 1:3)
    expect_equal(theta(truth), theta(model), tolerance=0.15)
    expect_equal(sigma(truth), colMeans(sigmac(model)), tolerance=0.1)
    expect_equal(p(truth), colMeans(pic(model)), tolerance=0.18)
    expect_identical(numberObs(truth), 1000L)
    if (FALSE) {
      library(purrr)
      mp <- McmcParams(iter = 1000,
                       burnin = 1000,
                       nStarts = 4,
                       thin=10)
      hp <- Hyperparameters(k=3,
                            tau2.0=0.4,
                            mu.0=-0.75,
                            eta.0=32,
                            m2.0=0.5)
      set.seed(5986)
      model <- gibbs(mp=mp, hp=hp, dat=y(truth), max_burnin=100000)
      ch <- ggSingleBatchChains(model)
      ch[[1]]
      ch[[2]]
      fig1 <- ggSingleBatch(model)
      fig2 <- ggSingleBatch(truth)
      library(gridExtra)
      grid.arrange(fig2, fig1, ncol=1)

      ##
      ## what happens when we over-specify the model?
      ## - expect warnings from label swapping
      expect_true(is.na(marginal_lik(SingleBatchModel2())))
      k(hp) <- 4
      expect_warning(model <- gibbs(mp=mp, hp=hp, dat=y(truth)))
      expect_true(is.na(marginal_lik(model)))
      ##
      ## what happens when we under-specify the model?
      ##
      k(hp) <- 2
      model <- gibbs(mp=mp, hp=hp, dat=y(truth))

      k(hp) <- 1
      model <- gibbs(mp=mp, hp=hp, dat=y(truth))

      model.list <- gibbs_K(hp, mp, dat=y(truth))
    }
})

test_that("test_marginal_Moderate", {
    set.seed(100)
    truth <- simulateData(N = 1000, theta = c(-2, -0.4, 0), sds = c(0.3,
        0.15, 0.15), p = c(0.05, 0.1, 0.8))
    ## verify that if we start at the true value, we remain in a region of
    ## high posterior probability after an arbitrary number of mcmc updates
    mcmcp <- McmcParams(iter = 250, burnin = 250, thin = 2, nStarts=0)
    model <- SingleBatchModel(y(truth), k = 3, mcmc.params = mcmcp)
    model <- startAtTrueValues(model, truth)
    model <- posteriorSimulation(model)
    expect_equal(theta(truth), theta(model), tolerance=0.15)
    expect_equal(sigma(truth), sigma(model), tolerance=0.15)
    expect_equal(p(truth), colMeans(pic(model)), tolerance=0.2)
  })

test_that("test_marginal_pooled", {
    set.seed(100)
    truth <- simulateData(N = 2500,
                          theta = c(-2, -0.4, 0),
                          sds = c(0.3, 0.3, 0.3),
                          p = c(0.05, 0.1, 0.8))
    mcmcp <- McmcParams(iter = 500, burnin = 500, thin = 2, nStarts=0)
    model <- SingleBatchModel(y(truth), k = 3, mcmc.params = mcmcp)
    model <- startAtTrueValues(model, truth)
    model <- posteriorSimulation(model)
    expect_equal(theta(truth), theta(model), tolerance=0.15)
    s2_pooled <- CNPBayes:::sigma2_pooled(model)
    nu0_pooled <- CNPBayes:::nu0_pooled(model)
    sigma20_pooled <- CNPBayes:::sigma2_0_pooled(model)
    s_pooled <- sqrt(s2_pooled)
    expect_equal(object=s_pooled, expected=0.3, tolerance=0.03)

    ylist <- split(y(model), z(model))
    tmp <- vector("list", length(ylist))
    for (i in 1:length(ylist)) {
        y <- ylist[[i]]
        th <- theta(model)[i]

        tmp[[i]] <- sum((y-th)^2)
    }
    r_ss <- sum(unlist(tmp))

    sigma2_n <- 0.5*(nu.0(model) * sigma2.0(model) + r_ss)
    nu_n <- length(y(model))
    set.seed(123)
    (sigma2_new <- 1/rgamma(1, 0.5*nu_n, sigma2_n))
    set.seed(123)
    (sigma2_new.cpp <- CNPBayes:::sigma2_pooled(model))
    expect_equal(sigma2_new, sigma2_new.cpp, tolerance=0.01)
})

test_that("test_marginal_pooled2", {
    set.seed(100)
    truth <- simulateData(N = 2500, theta = c(-2, -0.5, 0),
                          sds = c(0.1, 0.1, 0.1), p = c(0.05, 0.1, 0.8))
    pooled <- SingleBatchPooledVar(data=y(truth), k=3)
    pooled <- posteriorSimulationPooled(pooled, iter=1000, burnin=0, thin=1)
    expect_equal(theta(pooled), theta(truth), tolerance=0.01)
    expect_equal(sigma(pooled), sigma(truth)[3], tolerance=0.01)
    if(FALSE){
      plot.ts(sigmac(pooled), col="gray", ylim=c(0, 0.3))
      abline(h=mean(sigma(truth)))
      plot.ts(thetac(pooled), col="gray", plot.type="single")
      abline(h=theta(truth))
    }
  })


test_that("test_marginalEasy", {
    set.seed(1)
    truth <- simulateData(N = 2500, p = rep(1/3, 3), theta = c(-1,
        0, 1), sds = rep(0.1, 3))
    mp <- McmcParams(iter = 100, burnin = 0, nStarts = 20)
    model <- SingleBatchModel(data = y(truth), k = 3, mcmc.params = mp)
    model <- posteriorSimulation(model)
    if (FALSE) {
        SingleBatchModelExample <- model
        save(SingleBatchModelExample, file = "data/SingleBatchModelExample.RData")
    }
    expect_equal(theta(model), theta(truth), tolerance=0.03)
    expect_equal(sigma(model), sigma(truth), tolerance=0.11)
    expect_equal(p(model), p(truth), tolerance=0.05)
    i <- CNPBayes:::argMax(model)
    expect_true(i == which.max(logPrior(chains(model)) + log_lik(chains(model))))
    expect_identical(sort(CNPBayes:::thetac(model)[i, ]), modes(model)[["theta"]])
    expect_identical(sort(sigmac(model)[i, ]),
                     sort(sqrt(modes(model)[["sigma2"]])))
})

.test_that <- function(name, expr) {}

.test_that("test_segfaultExcept", {
  baf <- readRDS(system.file("extdata", "baf.rds", package = "CNPBayes"))
  set.seed(17)
  model <- SingleBatchModel(baf, k = 2)
  mcmcParams(model) <- McmcParams(iter=1000, nStarts=20, burnin=0)
  model@.internal.constraint <- -1
  ## this is a constraint on sigma2.0
  ##
  ## since this is negative, the proposed value of sigma2.0 would always be
  ## accepted
  ## -- the default constraint keeps this parameter from getting too close to
  ##    zero
  ## -- however, not clear what error was supposed to be triggered
  expect_error(model <- posteriorSimulation(model))
  ##model <- posteriorSimulation(model)
  ##sigma2.0(model)
})



test_that("test_selectK_easy", {
    set.seed(1)
    means <- c(-1, 0, 1)
    sds <- c(0.1, 0.2, 0.2)
    truth <- simulateData(N = 250, p = rep(1/3, 3), theta = means,
                          sds = sds)
    mp0 <- McmcParams(iter = 1, burnin = 0, nStarts = 1)
    mp <- McmcParams(iter = 1000, burnin = 50, nStarts = 20)
    model <- SingleBatchModel(data = y(truth), k = 2, mcmc.params = mp0)
    expect_error(mlist <- posteriorSimulation(model, k = 2:4))
    mlist <- SingleBatchModelList(data=y(truth), k=2:4, mcmc.params=mp)
    mlist2 <- posteriorSimulation(mlist)
##    expect_warning(mlist <- posteriorSimulation(mlist),
##                   "label switching: model k=4")
    if(FALSE){
      ##
      ## Visual inspection of the chains for theta shows that the k=4 model has
      ## not converged. This is probably because there is a lot of mixing in the
      ## overfit model. Computation of marg. lik. is only reasonable for
      ## converged models. The ML will be highly inflated in the overfit model
      ## that has not converged due to small p(theta*)
      ##
      mcmcParams(mlist) <- McmcParams(nStarts=0, iter=1000, thin=200, burnin=0)
      mp <- McmcParams(iter=1000, thin=10, burnin=0, nStarts=0)
      mod4 <- posteriorSimulation(mlist[[3]])
      plist4 <- ggSingleBatchChains(mlist[[3]])
      plist4[["comp"]]
      theta.ch <- thetac(mod4)
      theta.ch <- t(apply(theta.ch, 1, sort))
      theta.m <- data.frame(theta=as.numeric(theta.ch),
                            iter=rep(1:nrow(theta.ch), 4),
                            comp=rep(1:k(mod4), each=nrow(theta.ch)))
      theta.m$comp <- factor(theta.m$comp)
      ggplot(theta.m, aes(iter, theta, group=comp)) +
        geom_point(aes(color=comp), size=0.2) +
        geom_line(aes(color=comp))


      mod3 <- mlist[[2]]
      plist3 <- ggSingleBatchChains(mod3)
      plist3[["comp"]]

      mod2 <- mlist[[1]]
      plist2 <- ggSingleBatchChains(mod2)
      plist2[["comp"]]

      ## model 4 has now converged (see gelman diag), but pstar is not
      ## reasonable because of the mixing 
      library(coda)
      theta.chains <- thetac(mod4)
      theta.mc <- mcmc.list(mcmc(theta.chains[1:250, ]),
                            mcmc(theta.chains[751:1000, ]))
      gelman.diag(theta.mc)

      theta.chains <- thetac(mlist[[3]])
      tmp <- as.numeric(theta.chains)
      tmp2 <- kmeans(tmp, 4)


      theta.mc <- mcmc.list(mcmc(theta.chains[1:250, ]),
                            mcmc(theta.chains[751:1000, ]))
      gelman.diag(theta.mc)

      theta.mc <- mcmc.list(mcmc(theta.chains[1:250, 1]),
                            mcmc(theta.chains[751:1000, 1]))
      gelman.diag(theta.mc)
      theta.mc <- mcmc.list(mcmc(theta.chains[1:250, 2]),
                            mcmc(theta.chains[751:1000, 2]))
      gelman.diag(theta.mc)
      theta.mc <- mcmc.list(mcmc(theta.chains[, 1]),
                            mcmc(theta.chains[, 2]))

    }
    ##
    ## We get the correct answer only by diagnosing the lack of convergence
    ##
    ## -- the NAs returned by marginalLikelihood are an attempt to prevent
    ## -- erroneously calculating the marginal likelihood for a model that has
    ## -- not yet converged
    ##
    ##
    ##mlist <- mlist[1:2]
    m.y <- marginalLikelihood(mlist2)
    argmax <- which.max(m.y)
    expect_true(argmax == 2L)
})
