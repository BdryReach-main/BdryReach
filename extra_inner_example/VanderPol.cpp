/*
 * @Author: your name
 * @Date: 2021-11-11 17:24:27
 * @LastEditTime: 2021-12-28 18:18:30
 * @LastEditors: Please set LastEditors
 * @Description: 打开koroFileHeader查看配置 进行设置:
 * https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 * @FilePath: /Solver/test.cpp
 */

#include <plotter/matplotlibcpp.h>
#include <plotter/plotter.h>
#include <underApprox/underApprox.h>

namespace plt = matplotlibcpp;

using namespace std;
using namespace reachSolver;
using namespace capd;
using capd::autodiff::Node;

double mu = 1.;

void _f(Node/* t*/, Node in[], int /*dimIn*/, Node out[], int/* dimOut*/, Node params[], int noParams){
    out[0] = in[1];
    out[1] = mu * (1-in[0]*in[0])*in[1] - in[0];
}

//因为要对最后的结果进行检验，因此要定义反向微分方程 dx/dt = -f(x)
void _fBack(Node/* t*/, Node in[], int /*dimIn*/, Node out[], int/* dimOut*/, Node params[], int noParams){
    out[0] = -in[1];
    out[1] = -(mu * (1-in[0]*in[0])*in[1] - in[0]);
}


int dimIn = 3;

int dimOut = 2;
int noParam = 0;
int MaxDerivativeOrder = 3;

IMap f(_f, dimIn,dimOut,noParam,MaxDerivativeOrder);
IMap fBack(_fBack, dimIn,dimOut,noParam,MaxDerivativeOrder);
int main()
{
    NonlinearSys<double> mysys(f, 2, 0, 2);
	NonlinearSys<double> mysysBack(fBack, 2, 0, 2);

    ReachOptions<double> options;

    //create R0
    Vector_t<double> center(2);
    center << 1.4, 2.4;
    Matrix_t<double> generators(2,2);
    generators<< 0.17,0,
                 0,0.06;

    Zonotope<double> R0_(center,generators);

    options.set_taylor_terms(4);
    options.set_zonotope_order(50);
    options.set_intermediate_order(50);
    options.set_error_order(20);
    options.set_alg("lin");
    options.set_tensor_order(3);

	options.set_tFinal(6);
    options.set_tStart(0);

    options.set_R0(R0_);

    options.set_usekrylovError(1);
    options.set_max_error(DBL_MAX*Eigen::MatrixXd::Ones(2,1));
	
	
	plt::figure_size(1200, 780);

    clock_t start, end;
	start = clock();

    //0.8s 19.97s
	// vector<Zonotope<double>> underR = UnderApprox::underReachClp(mysys, mysysBack, options, R0_, 0.1, 8, 0.01, 0.05, 0.01, 50, 50);

//     [2.014240788670479 0.04268362586059616 -1.313266984121001e-07 -3.266235373498935e-06 3.167427387495068e-09 8.91414822980811e-08 -2.782309050327176e-06 -9.886263565836389e-08 -1.848919048031407e-09 -5.454233484856422e-11 7.404207736935861e-08 2.184210163804997e-09 -6.484333812653727e-08 -2.198110574711707e-06 -1.825279529419029e-09 -4.785164387818503e-11 1.754102723770856e-09 6.69094629715894e-08 -4.974739464003001e-08 -1.897592093377854e-06 -1.637535302645917e-09 -3.785424096059824e-11 5.664617325976684e-08 1.309466665302586e-09 -3.578009737034424e-08 -1.547809996693196e-06 -1.550020763724365e-09 5.161343702860245e-08 1.222838886700498e-09 -3.245785830802162e-08 -1.369977389568153e-06 1.299095978074865e-09 1.095962580376981e-10 -1.348764536841183e-09 1.031079168746759e-09 4.391061664741498e-08 -1.143116857657054e-06 -2.684189085381084e-08 1.96255044637308e-05 4.792100024857571e-07 3.92389847953424e-08 9.581264031232979e-10 -1.219677725690895e-09 -2.465843302900828e-08 -1.009858277100116e-06 2.180972933606952e-05 5.121211978185738e-07 2.547712678537982e-05 6.036106709440131e-07 2.771809111422444e-05 6.407478961279813e-07 3.211105037325105e-05 8.418253326406097e-07 1.010611078568e-06 3.425849073942103e-05 1.375073777677072e-06 3.869895022543658e-05 3.916929988759259e-05 1.574894104413708e-06 2.057882766172576e-06 4.17924856345589e-05 3.920300687467346e-05 2.204030979749278e-06 2.797794515630366e-06 3.853777265774376e-05 3.32275662888172e-05 2.803193136161084e-06 3.012263265587289e-05 3.045566529304621e-06 2.642437805381012e-06 2.289537029591354e-05 1.795516529439614e-05 2.270178563495406e-06 1.388162596224725e-05 1.552373216382436e-06 0 5.877531125407519e-11 4.728361755936457e-05 2.880406589030115e-09 1.11028167697844e-06 0.0001203220471186935 1.740930010599293e-10 2.850701030869363e-06 7.37846706237965e-09 0.0002245507224214238 1.385793725091162e-08 3.593261195543531e-10 4.966215976086673e-06 0.0002148333654311254 2.300762173327102e-08 6.871154783138257e-10 0.0003646203692636671 9.558910719987517e-06 ;
// -0.2907130541786854 -0.06501330256935019 2.045779066486604e-07 5.088071225512902e-06 -5.183614440873887e-09 -1.45883399488651e-07 5.456222856886787e-06 1.938737079936555e-07 3.862064411740606e-09 1.139292769881731e-10 -1.582957158829778e-07 -4.669657089625001e-09 1.506968399439313e-07 5.108440234985817e-06 4.596088012830081e-09 1.204913347670875e-10 -4.44342979057409e-09 -1.694926397469711e-07 1.305446921972472e-07 4.979568830457096e-06 4.583420801282834e-09 1.059530839764099e-10 -1.587633389342009e-07 -3.67006786236222e-09 1.016021376474072e-07 4.395203364270274e-06 4.607759036834926e-09 -1.534624297205823e-07 -3.635871538759867e-09 9.685985384618629e-08 4.088249091079332e-06 -3.940587084613515e-09 -3.324416411367227e-10 4.134564547015297e-09 -3.160747469017131e-09 -1.346068998755869e-07 3.505835190555324e-06 8.232163221633377e-08 -6.059149662600697e-05 -1.479505981740497e-06 -1.219677725690895e-07 -2.978174482293265e-09 3.791163873730291e-09 7.665011224986382e-08 3.139118783627392e-06 -6.845615505224754e-05 -1.607440770272716e-06 -8.145436602637088e-05 -1.929837887242118e-06 -9.039617861678431e-05 -2.089651882159723e-06 -0.0001070781004765392 -2.807166271561021e-06 -3.447796445826548e-06 -0.0001168761209090684 -4.809399511623332e-06 -0.0001353518009986074 -0.0001403849351087073 -5.644507491471346e-06 -7.580775108503466e-06 -0.0001539540735890414 -0.0001480004682225354 -8.320729530331059e-06 -1.083640783200322e-05 -0.0001492643648857262 -0.0001322524105189613 -1.115727363795163e-05 -0.0001242124843038529 -1.255857643783454e-05 -1.138873116993962e-05 -9.867752300750299e-05 -8.248456131255413e-05 -1.042901470639905e-05 -6.997647146095282e-05 -7.825423359508783e-06 0.0008042402996602117 1.087172355473248e-09 0.0008746094368619176 5.327914645282993e-08 2.053698262492393e-05 0.001026488399392388 1.485217799088296e-09 2.431982839717412e-05 6.294699123062748e-08 0.0005888553594240156 7.2471415686241e-08 1.879130501578393e-09 2.597130408906654e-05 0.001123491746826102 8.267435111810374e-08 2.469043822580472e-09 0.001310207234054319 3.434847592387375e-05 ;]

    //2s 178.984614s
    // vector<Zonotope<double>> underR = UnderApprox::underReachClp(mysys, mysysBack, options, R0_, 0.1, 20, 0.01, 0.005, 0.005, 50, 50);

    //2.5s 210s
    // vector<Zonotope<double>> underR = UnderApprox::underReachClp(mysys, mysysBack, options, R0_, 0.1, 25, 0.01, 0.005, 0.005, 50, 50);
//     [0.6065230026685057 0.04301302920814006 3.511211981727971e-06 1.8796273490245e-05 4.886040836881092e-05 4.98504953093448e-05 5.085980690464904e-05 5.052824129618027e-05 6.193454803006249e-06 7.744318120192411e-05 7.882834906336554e-05 8.020172394719877e-05 7.917014480013549e-05 8.072333855324396e-05 0.0004755999419205666 0.0006255361091751068 0.0001153635898211392 0.0001172319385421329 4.884570064292988e-05 0.0004579088449742467 0.0003191408356614568 0.000327295418835959 0.0001504025265319209 0.0003354678499634274 0.0002317678109029678 0.0002139420814395219 0.0002129027711824053 0.0003434459581164019 0.0002117413131749098 0.0002104557214630826 0.0002090964762683191 0.0001763806035936217 0.0002084740175581942 0.0001781648197400628 0.0002082058344565299 0.0001799138146579477 0.0002078334472689113 0.000181626576972991 0.0002073546136557173 0.0001952503364964369 0.0001726787942562707 0.0001988916455130195 4.404012964009496e-05 ;
// -1.537280344817128 0.03916150470619461 3.316199803780927e-06 1.956016206202576e-05 5.081068622079482e-05 5.180460413493407e-05 5.28175318523285e-05 5.243794389489443e-05 6.368939864468389e-06 7.959344522273086e-05 8.097285018071985e-05 8.233916769344259e-05 8.123680689664155e-05 8.278696937095749e-05 0.0004875048257126719 0.0006395896823296413 0.000117624328705598 0.0001194756644052299 4.975850482035736e-05 0.0004645340698735878 0.000323385793779227 0.0003314018078050659 0.0001508276666319205 0.0003394296202465122 0.0002324844552814498 0.000214660955256366 0.0002136757991839133 0.0003472549172064725 0.0002125680412147817 0.0002113355886670875 0.0002100290445896396 0.0001782128578968763 0.0002094626302542624 0.0001799541424406349 0.0002092525416362033 0.0001816593579556808 0.0002089381744129862 0.0001833275287819792 0.0002085171919961213 0.0001970138370983449 0.0001741815701956255 0.000200557804510381 4.439489121991092e-05 ;]
    
    //2.8s 221s
    // vector<Zonotope<double>> underR = UnderApprox::underReachClp(mysys, mysysBack, options, R0_, 0.1, 28, 0.01, 0.005, 0.005, 50, 50);

//     [0.06178125945375713 0.05429615890419663 5.908321454623428e-08 3.434376950341097e-09 6.049884915573204e-08 3.464391916091743e-09 6.192933365507497e-08 3.493891495542412e-09 6.337161804376112e-08 3.522838296776948e-09 6.482826008456207e-08 3.5511935976496e-09 7.652977337354092e-11 4.722040828155056e-06 3.578918290826719e-09 6.629274442902732e-08 6.776608671005014e-08 3.605971079580722e-09 0.0001221313452422753 0.0002854679572805186 0.0002873036832783906 0.0002890388155396475 0.0002906044723968628 0.0002920033984382251 0.0003163121025635741 0.000205252808569235 3.632309739935791e-09 6.925863377968987e-08 7.076408480728152e-08 3.657891691303969e-09 7.228190474830831e-08 3.682672740393115e-09 7.381093401759858e-08 3.706607760268538e-09 ;
// -2.112004068090411 0.05124998614225527 5.586826270331278e-08 3.247498890462584e-09 5.750104868013763e-08 3.292726572392868e-09 5.916323909804525e-08 3.337835654501079e-09 6.085225244759701e-08 3.382786363755127e-09 6.257086439776962e-08 3.427536891456307e-09 7.393373433801758e-11 4.572796767642672e-06 3.472044265560172e-09 6.431310369197658e-08 6.608021776166396e-08 3.516262569513516e-09 0.0001234507986491877 0.0002884962101173278 0.0002902958083446033 0.0002919936420760337 0.0002935201957536129 0.0002948783356842597 0.0003193675733673888 0.0002071977018352791 3.560144111732103e-09 6.788262425046706e-08 6.971455270034271e-08 3.603639950690302e-09 7.157582045199228e-08 3.646698627653322e-09 7.346561734276262e-08 3.68926678655329e-09 ;]

    //2.8s 202s
    // vector<Zonotope<double>> underR = UnderApprox::underReachClp(mysys, mysysBack, options, R0_, 0.2, 14, 0.01, 0.005, 0.005, 50, 50);

//     [0.0634019309799507 0.05552898802833001 6.680503925607911e-08 6.841963438093019e-08 0.001135642607809143 7.004990964866258e-08 8.358196770942629e-08 7.169576919221293e-08 8.184930471148008e-08 7.335868941717916e-08 8.01314733011428e-08 2.195728787349747e-06 7.503505390823799e-08 7.841889708886011e-08 7.671740248536693e-08 ;
// -2.110379144296938 0.05249138797137412 6.315072131413881e-08 6.500967601858696e-08 0.001136975557854676 6.690094657555694e-08 8.316663891367654e-08 6.882481814631819e-08 8.102600082399855e-08 7.078309645930777e-08 7.891978052790135e-08 2.12810911891418e-06 7.277268271826116e-08 7.683819269539732e-08 7.478666476789248e-08 ;]

    vector<Zonotope<double>> underR = UnderApprox::underReachClp(mysys, mysysBack, options, R0_, 0.5, 2, 0.05, 0.01, 0.005, 50, 50);

    end = clock();
	cout << "time cost: " << (double) (end - start) / CLOCKS_PER_SEC << endl;

	for(int i = 1; i < underR.size(); i++){
		Plotter::plotZonotope(underR[i], 1, 2, "g");
	}
	Plotter::plotZonotope(R0_, 1, 2, "k");
	plt::show();
}