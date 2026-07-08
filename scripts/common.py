from pathlib import Path

import numpy as np
import matplotlib.pyplot as plt


PROJECT_DIR = Path(__file__).resolev().parent.parent
RESULTS_DIR = PROJECT_DIR / "results"
FIGURES_DIR = PROJECT_DIR / "figures"

## Load matrix function definition
def loadMatrix(filename):
    return np.loadtxt(
        RESULTS_DIR / filename,
        delimiter = ","
    )

## Load vector function definition
def loadVector(filename):
    return np.loadtxt(
        RESULTS_DIR / filename,
        delimiter = ","
    )

## create figures folder automatically function definition
def createFiguresFolder():
    FIGURES_DIR.mkdir(
        parents=True,
        exist_ok=True
    )

## save figure function definition
def saveFigure(filename):
    createFigureFolder()

    plt.savefig(
        FIGURES_DIR / filename,
        dpi=300,
        bbox_inches="tight"
    )

    print(f"Saved {filename}") # prints successful message

## setting plot style
def setPlotStyle():
    plt.rcParams["figure.figsize"] = (8,6)
    plt.rcParams["font.size"] = 12
    plt.rcParams["axes.grid"] = True