"""
#############################################################
# Program showing how to use interactive windows and MINUIT #
#############################################################
"""


import sys
import threading

from random   import seed, random
from time     import sleep
from array    import array
from math     import exp, sqrt
from ctypes   import c_int, c_double
from argparse import ArgumentParser

from ROOT import gROOT, gStyle, gPad, gApplication, TGaxis, TCanvas, TF1, TH1F, TGMainFrame, TPyDispatcher, TRootEmbeddedCanvas, TGLayoutHints, TGHorizontalFrame, TGTextButton, TGClient, kLHintsExpandX, kLHintsExpandY, TMinuit, TString


class myGui(TGMainFrame):
    def __init__(self, parent, width, height):
        self.draw = TPyDispatcher(self.drawHisto)
        self.out  = TPyDispatcher(self.getOut)

        TGMainFrame.__init__(self, parent, width, height)

        self.Canvas = TRootEmbeddedCanvas('Canvas', self, 800, 500)
        self.AddFrame(self.Canvas, TGLayoutHints(kLHintsExpandX | kLHintsExpandY))
        self.ButtonsFrame = TGHorizontalFrame(self, 200, 40)

        # Draw button
        self.DrawButton = TGTextButton(self.ButtonsFrame, '&Draw', 10)
        self.DrawButton.Connect('Clicked()', 'TPyDispatcher', self.draw, 'Dispatch()')
        self.ButtonsFrame.AddFrame(self.DrawButton, TGLayoutHints())

        # Exit button
        self.DrawButton = TGTextButton(self.ButtonsFrame, '&Exit', 10)
        self.DrawButton.Connect('Clicked()', 'TPyDispatcher', self.out, 'Dispatch()')
        self.ButtonsFrame.AddFrame(self.DrawButton, TGLayoutHints())

        self.AddFrame(self.ButtonsFrame, TGLayoutHints())

        self.SetWindowName('My Window')
        self.MapSubwindows()
        self.Resize(self.GetDefaultSize())
        self.MapWindow()

        self.myHisto = []

    def __del__(self):
        self.Cleanup()

    def getOut(self):
        self.__del__()

    def drawHisto(self):
        if self.myHisto == 0:
            return

        self.Canvas.GetCanvas().cd()

        for i,H in enumerate(self.myHisto):
            H.SetLineColor(int(10 * random()))

            if i == 0:
                H.Draw()
            else:
                H.Draw('same')

        gPad.Update()


def eventLoop(myWindow, ID, nEvents):
    print('\n=== Event loop', ID, 'started ===')

    ev = 0
    while ev < nEvents:
        print('Event #', ev)
        ev += 1

        myWindow.drawHisto()
        sleep(0.5)

    print('=== Event loop', ID, 'finished ===')


"""
################
# Fit function #
################
"""
def myFitFun(x, par):
    return par[2] * exp(-(x[0]-par[0]) * (x[0]-par[0]) / (2*par[1]*par[1]))

xVal    = []
yVal    = []
erryVal = []

# Functional to maximise
def myFunctional(npar, grad, fval, par, iflag):
    # Calculate chi2
    chi2, delta = 0., 0.
    for i in xrange(len(xVal)):
        delta = (yVal[i] - myFitFun([xVal[i]], par)) / erryVal[i] if erryVal[i] != 0 else 0
        chi2 += delta * delta
    fval.value = chi2


def ArgParser(argv):
    """
    ###################
    # Argument parser #
    ###################
    """
    parser = ArgumentParser()
    parser.add_argument('-i', '--input',    dest = 'input',     help = 'input file',  default = ''        )
    parser.add_argument('-o', '--output',   dest = 'output',    help = 'otuput file', default = ''        )
    parser.add_argument('-w', '--whatever', dest = 'whaterver', help = 'whatever',    action='store_false')

    options = parser.parse_args()
    if not options.input:
        parser.error('No -input- parameter given')
    else:
        print('I\'m reading the -intput- option: ', options.input)

    if not options.output:
        parser.error('No -output- parameter given')
    else:
        print('I\'m reading the -output- option: ', options.output)

    return options


def SetStyle():
    gROOT.SetStyle('Plain')
    gROOT.ForceStyle()
    gStyle.SetTextFont(42)

    gStyle.SetOptTitle(0)
    gStyle.SetOptFit(1112)
    gStyle.SetOptStat(1110)

    gStyle.SetPadRightMargin(0.08)
    gStyle.SetPadTopMargin(0.11)
    gStyle.SetPadBottomMargin(0.12)

    gStyle.SetTitleFont(42,'x')
    gStyle.SetTitleFont(42,'y')
    gStyle.SetTitleFont(42,'z')

    gStyle.SetTitleOffset(1.05,'x')
    gStyle.SetTitleOffset(0.95,'y')

    gStyle.SetTitleSize(0.05,'x')
    gStyle.SetTitleSize(0.05,'y')
    gStyle.SetTitleSize(0.05,'z')

    gStyle.SetLabelFont(42,'x')
    gStyle.SetLabelFont(42,'y')
    gStyle.SetLabelFont(42,'z')

    gStyle.SetLabelSize(0.05,'x')
    gStyle.SetLabelSize(0.05,'y')
    gStyle.SetLabelSize(0.05,'z')

    TGaxis.SetMaxDigits(3)
    gStyle.SetStatY(0.9)


"""
################
# Main program #
################
"""
def main(argv = None):
    """
    #######################
    # Internal parameters #
    #######################
    """
    nBins = 100
    seed(0)


    """
    #################################
    # Command line argument parsing #
    #################################
    """
    if argv is None:
        argv = sys.argv
    else:
        print('=== Some arguments are needed, for help run: python TestPyROOT.py --help')
        quit(0)
    cmd = ArgParser(argv)


    """
    ############
    # Graphycs #
    ############
    """
    gROOT.Reset()
    SetStyle()


    # Dynamic window
    myWindow = myGui(TGClient.Instance().GetRoot(), 800, 500)

    funMyWindow = TF1('funMyWindow', '[2]*exp(-(x-[0])*(x-[0])/(2*[1]*[1]))', 0, 10)
    funMyWindow.SetParNames('#mu','#sigma','Scale')
    funMyWindow.SetParameters(1, 1, 1)

    myWindow.myHisto.append(TH1F('hisMyWindow', 'hisMyWindow', nBins, 0 , 10))
    myWindow.myHisto[-1].FillRandom('funMyWindow',1000)


    """
    ###############
    # Call MINUIT #
    ###############
    """
    xVal.extend(   [ myWindow.myHisto[-1].GetBinCenter(i)        for i in xrange(nBins) ])
    yVal.extend(   [ myWindow.myHisto[-1].GetBinContent(i)       for i in xrange(nBins) ])
    erryVal.extend([ sqrt(myWindow.myHisto[-1].GetBinContent(i)) for i in xrange(nBins) ])

    gMinuit = TMinuit(nBins)
    gMinuit.SetFCN(myFunctional)

    arglist = array('d', nBins*[0])
    ierflg  = c_int(0)

    arglist[0] = 1 # 1 for chi2, 0.5 for likelihood
    gMinuit.mnexcm('SET ERR', arglist, 1, ierflg)

    # Set starting values and step sizes for parameters
    nParams = 3
    vStart  = [1   for i in range(nParams)]
    vStep   = [0.1 for i in range(nParams)]
    gMinuit.mnparm(0, '#mu',    vStart[0], vStep[0], 0, 0, ierflg)
    gMinuit.mnparm(1, '#sigma', vStart[1], vStep[1], 0, 0, ierflg)
    gMinuit.mnparm(2, 'Scale',  vStart[2], vStep[2], 0, 0, ierflg)

    # Fix parameters (counting from 1)
#    arglist[0] = 1
#    gMinuit.mnexcm('FIX', arglist, 1, ierflg)

    # Define printout level
    arglist[0] = 0
    # -1 = no output except from SHOW
    #  0 = minimum output (no starting values or intermediate results) default value, normal output
    #  1 = additional output giving intermediate results.
    #  2 = maximum output, showing progress of minimizations.
    gMinuit.mnexcm('SET PRI', arglist, 1, ierflg)

    # Now ready for minimization step
    arglist[0] = 500 # Max calls
    arglist[1] = 1.0 # Tolerance
    gMinuit.mnexcm('MIGRAD', arglist, 2, ierflg)

    # Print results
    fmin, fedm, errdef  = c_double(0.), c_double(0.), c_double(0.)
    npari, nparx, istat = c_int(0), c_int(0), c_int(0)
    gMinuit.mnstat(fmin, fedm, errdef, npari, nparx, istat)
    print('\nFMIN:', round(fmin.value,2), '\tFEDM:', '{:.1e}'.format(fedm.value), '\tERRDEF:', errdef.value, '\tNPARI:', npari.value, '\tNPARX:', nparx.value, '\tISTAT:', istat.value)

    # Extract parameters and errors
    val, err, errLo, errHi = c_double(0.), c_double(0.), c_double(0.), c_double(0.)
    parVal, parErr, parErrLo, parErrHi = [0 for i in range(nParams)], [0 for i in range(nParams)], [0 for i in range(nParams)], [0 for i in range(nParams)]
    for i in range(nParams):
        gMinuit.mnpout(i, TString(''), val, err, errLo, errHi, ierflg)
        parVal[i]   = val.value
        parErr[i]   = err.value
        parErrLo[i] = errLo.value
        parErrHi[i] = errHi.value


    myWindow.myHisto[-1].Fit('funMyWindow')
    myWindow.myHisto.append(myWindow.myHisto[-1].GetFunction('funMyWindow'))


    # Static window
    myCanv = TCanvas('myCanv', 'ROOT Window', 10, 10, 800, 500)
    funCanv = TF1('funCanv', 'abs(sin(x)/x)', 0, 10)
    funCanv.Draw()
    myCanv.Modified()
    myCanv.Update()


    """
    ################################
    # Greate the event loop thread #
    ################################
    """
    myThread = threading.Thread(target = eventLoop, args = (myWindow, 1, 10))
    myThread.start()


    """
    ############################
    # Wait for keyborad stroke #
    ############################
    """
    gApplication.Run()


if __name__ == '__main__':
    sys.exit(main())
