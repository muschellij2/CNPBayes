context("Constructors")

test_that("test_constructor", {
    mcmc.params <- McmcParams()
    expect_true(validObject(mcmc.params))
    hypp <- Hyperparameters()
    expect_true(validObject(hypp))
    hypp <- Hyperparameters("batch")
    expect_true(validObject(hypp))
    ##expect_warning(mmod <- SingleBatchModel())
    mmod <- SingleBatchModel()
    expect_true(validObject(mmod))
    mc <- CNPBayes:::McmcChains()
    expect_true(validObject(mc))
    hp <- Hyperparameters()
    expect_true(validObject(hp))
    expect_true(validObject(HyperparametersMultiBatch()))
    expect_true(validObject(HyperparametersMarginal()))
    truth <- simulateData(N = 2500, p = rep(1/3, 3), theta = c(-1,
        0, 1), sds = rep(0.1, 3))
    expect_true(validObject(truth))
    mmod <- SingleBatchModel(data = rnorm(100))
    mmod <- SingleBatchModel(data = y(truth))
    expect_true(validObject(mmod))
    expect_true(iter(mmod) == 1000)
    iter(mmod) <- 1000L
    expect_error(iter(mmod) <- 1001)
    iter(mmod, force = TRUE) <- 1001L
    expect_true(iter(mmod) == 1001L)
    expect_true(burnin(mmod) == 100L)
    expect_true(nStarts(mmod) == 1L)
    expect_true(nrow(CNPBayes:::thetac(mmod)) == 1001)
    mp <- McmcParams(iter = 10L, thin = 10L, burnin = 100L)
    mcmcParams(mmod) <- mp
    expect_true(nrow(CNPBayes:::thetac(mmod)) == 10)
    iter(mmod, force = TRUE) <- 1000L
    expect_true(nrow(CNPBayes:::thetac(mmod)) == 1000)
    bmod <- MultiBatchModel()
    expect_true(validObject(bmod))
    k <- nbatch <- 3
    means <- matrix(c(-1.2, -1, -0.8, -0.2, 0, 0.2, 0.8, 1, 1.2),
        nbatch, k, byrow = FALSE)
    sds <- matrix(0.1, nbatch, k)
    truth <- simulateBatchData(N = 2500, batch = rep(letters[1:3],
        length.out = 2500), theta = means, sds = sds, p = c(1/5,
        1/3, 1 - 1/3 - 1/5))
    expect_true(validObject(truth))
    bmod <- MultiBatchModel(data = y(truth), k = 3)
    expect_true(is(bmod, "SingleBatchModel"))
    bmod <- MultiBatchModel(data = y(truth), batch = batch(truth))
    expect_true(is(bmod, "MultiBatchModel"))
    expect_equal(y(bmod), y(truth))
    expect_equal(batch(bmod), batch(truth))
    expect_error(MultiBatchModel(data = y(truth), batch = rep(1:3,
        each = 2), k = 3))
    expect_true(iter(bmod) == 1000L)
    expect_true(burnin(bmod) == 100L)
    expect_true(nStarts(bmod) == 1L)
    expect_true(nrow(CNPBayes:::thetac(bmod)) == 1000)
    expect_error(iter(bmod) <- 10L)
    iter(bmod, force = TRUE) <- 10L
    expect_true(nrow(CNPBayes:::thetac(bmod)) == 10)
    iter(mmod, force = TRUE) <- 5L
    burnin(mmod) <- 0L
    mmod2 <- posteriorSimulation(mmod)
    expect_false(identical(CNPBayes:::thetac(mmod), CNPBayes:::thetac(mmod2)))
    expect_true(all(is.na(CNPBayes:::thetac(mmod))))
    burnin(mmod2) <- 2L
    expect_false(all(is.na(CNPBayes:::thetac(mmod2))))

    truth <- simulateData(N = 2500, p = rep(1/3, 3),
                          theta = c(-1, 0, 1), sds = rep(0.1, 3))
    pooled.model <- CNPBayes:::SingleBatchPooledVar(data=y(truth))
    expect_true(validObject(pooled.model))
})
