#include <catch2/catch.hpp>
#include "MEKF.hpp"
#include "Parameters.hpp"

using namespace Eigen;

const SatelliteModel satelliteModel;

TEST_CASE("MEKF predict test") {
    MEKF mekf;

    Vector3f gyroMeasurements;
    gyroMeasurements << 0.0142643813238961, 0.14848215231781, -0.0791417585254324;

    GlobalStateVector globalState;
    globalState
            << 0.4486483147627, -0.114193231757218, 0.886315639304009, -0.0109170969839343, 0.0131055903551422, 0.149087220992626, -0.0793308773361279;
    mekf.setGlobalState(globalState);

    Matrix<float, LocalStateSize, LocalStateSize> P;
    P
            << 0.000337179933164061, -1.35958687441977e-05, -5.91609214736156e-05, -8.7060924123911e-05, 3.80878717796191e-06, 1.55817175081154e-05,
            -1.35958687441977e-05, 0.000204930906168386, -1.30855063776812e-05, 3.71914105746695e-06, -5.14131269453151e-05, 3.34122716765293e-06,
            -5.91609214736156e-05, -1.30855063776811e-05, 0.000180318993884238, 1.56320443233367e-05, 3.28544286743409e-06, -4.50085267297301e-05,
            -8.70609241239109e-05, 3.71914105746693e-06, 1.56320443233367e-05, 0.000281148857843523, -2.25300491524147e-06, -6.58680595567629e-06,
            3.80878717796192e-06, -5.14131269453151e-05, 3.2854428674342e-06, -2.25300491524158e-06, 0.000264126918125299, -1.38895510411584e-06,
            1.55817175081154e-05, 3.3412271676528e-06, -4.50085267297301e-05, -6.58680595567635e-06, -1.38895510411517e-06, 0.000260333032004579;

    mekf.setP(P);

    mekf.predict(Parameters::SatelliteModel::Timestep, gyroMeasurements);

    GlobalStateVector expectedState;
    expectedState
            << 0.4486483147627, -0.114193231757218, 0.886315639304009, -0.0109170969839343, 0.0131055903551422, 0.149087220992626, -0.0793308773361279;

    Matrix<float, LocalStateSize, LocalStateSize> expectedP;
    expectedP
            << 0.000457395623188004, -1.43817488333673e-05, -6.2356658766682e-05, -0.000115174771470664, 4.03306879457865e-06, 1.62369515817417e-05,
            -1.43817488333673e-05, 0.00031785218777148, -1.3763005969784e-05, 3.94820378119138e-06, -7.78255037583639e-05, 3.47309781676073e-06,
            -6.23566587666821e-05, -1.37630059697839e-05, 0.000291934626339302, 1.62963990067256e-05, 3.43158916032775e-06, -7.10431874102803e-05,
            -0.000115174771470664, 3.94820378119134e-06, 1.62963990067256e-05, 0.000281248857843523, -2.25300491524147e-06, -6.58680595567629e-06,
            4.03306879457866e-06, -7.78255037583639e-05, 3.43158916032792e-06, -2.25300491524158e-06, 0.000264226918125299, -1.38895510411584e-06,
            1.62369515817417e-05, 3.47309781676054e-06, -7.10431874102802e-05, -6.58680595567635e-06, -1.38895510411517e-06, 0.000260433032004579;

    auto outputState = mekf.getGlobalState();
    auto outputP = mekf.getP();

    for (int i = 0; i < globalState.size(); i++) {
        REQUIRE(outputState(i) == Approx(expectedState(i)).epsilon(0.01));
    }
    for (int i = 0; i < LocalStateSize; i++) {
        for (int j = 0; j < LocalStateSize; j++) {
            REQUIRE(outputP(i, j) == Approx(expectedP(i, j)).epsilon(0.01));
        }
    }
}

TEST_CASE("MEKF correct test - Without eclipse") {
    Matrix<float, LocalStateSize, LocalStateSize> Q;
    Q << 1.0000, 0, 0, 0, 0, 0,
            0, 1.0000, 0, 0, 0, 0,
            0, 0, 1.0000, 0, 0, 0,
            0, 0, 0, 0.0010, 0, 0,
            0, 0, 0, 0, 0.0010, 0,
            0, 0, 0, 0, 0, 0.0010;
    Q *= 0.0001;
    Matrix<float, MeasurementSize, MeasurementSize> R;
    R << 0.5000, 0, 0, 0, 0, 0,
            0, 0.5000, 0, 0, 0, 0,
            0, 0, 0.5000, 0, 0, 0,
            0, 0, 0, 1.000, 0, 0,
            0, 0, 0, 0, 1.0000, 0,
            0, 0, 0, 0, 0, 1.0000;
    R *= 0.001;

    MEKF mekf;
    mekf.setQ(Q);
    mekf.setR(R);

    MeasurementVector measurements(0.95096940533737, -0.0591251014318659, -0.303580981770863, -0.0549449466873172,
                                   0.944547804661188, -0.323744494228485);

    Vector3f magneticFieldECI(-36354.0632660979, -13855.5244596001, -26590.0108895654);

    Vector3f sunPositionECI(-0.175103910047596, 0.918866877363553, 0.398319329957772);

    bool eclipse = false;

    Matrix<float, LocalStateSize, LocalStateSize> P;
    P
            << 0.000870224300067165, -0.000201808316484561, -9.94737304030683e-05, -0.000823536427216137, 0.00019790714175496, 9.94747338020742e-05,
            -0.000201808316484561, 0.000511536169607892, 1.04891056336985e-05, 0.000195831694631971, -0.000478747395815867, -1.51251051804663e-05,
            -9.94737304030679e-05, 1.04891056336982e-05, 0.000384104019745898, 9.46842077203422e-05, -7.09490031375223e-06, -0.000353368311599475,
            -0.000823536427216131, 0.000195831694631972, 9.46842077203436e-05, 0.00194059834870706, -0.00027847161419652, -0.000129706181113663,
            0.00019790714175496, -0.000478747395815865, -7.09490031375249e-06, -0.000278471614196521, 0.00146831434271805, 1.37244535870129e-05,
            9.94747338020709e-05, -1.5125105180466e-05, -0.000353368311599474, -0.000129706181113652, 1.37244535870129e-05, 0.00128677590871727;

    GlobalStateVector currentState(0.417973959608251, -0.070366169062251, 0.904098947785845, 0.054327377568127,
                                   0.0155935384360438, 0.148316004398812, -0.0832557864677611);

    mekf.setP(P);
    mekf.setGlobalState(currentState);

    GlobalStateVector expectedState(0.419543338867396, -0.0723533634494709, 0.903348658620409, 0.0520536125379745,
                                    0.0131084960408313, 0.151017030517226, -0.0779175696843138);

    Matrix<float, LocalStateSize, LocalStateSize> expectedP;
    expectedP
            << 0.00048521878227115, -0.000122665712867073, -6.20122609113396e-05, -0.000459304885817903, 0.000119724890527455, 6.15531964656011e-05,
            -0.000122665712867073, 0.000268604946327022, 6.85766789136854e-06, 0.000118589984422999, -0.000251585328330496, -9.30794393067327e-06,
            -6.20122609113394e-05, 6.85766789136847e-06, 0.000190294683949429, 5.89402248147446e-05, -5.2228565411889e-06, -0.00017517707393504,
            -0.0004593048858179, 0.000118589984422999, 5.89402248147451e-05, 0.00159599446903592, -0.000202304764827346, -9.35335667013269e-05,
            0.000119724890527456, -0.000251585328330496, -5.22285654118899e-06, -0.000202304764827347, 0.00125582934318329, 9.65157739458898e-06,
            6.15531964655994e-05, -9.30794393067315e-06, -0.00017517707393504, -9.35335667013187e-05, 9.65157739458904e-06, 0.00112289854007941;

    Vector3f satPositionECI(1.0e+06 * 4.569033859736713, 1.0e+06 * 0.261242083257025, -1.0e+06 * 5.135087257503072);
    float albedo = 1.000567064204868e-10;

    mekf.correct(measurements, magneticFieldECI, sunPositionECI, eclipse, satPositionECI, albedo);

    auto outputState = mekf.getGlobalState();
    auto outputP = mekf.getP();

    for (int i = 0; i < currentState.size(); i++) {
        REQUIRE(outputState(i) == Approx(expectedState(i)).epsilon(0.01));
    }
    for (int i = 0; i < LocalStateSize; i++) {
        for (int j = 0; j < LocalStateSize; j++) {
            REQUIRE(outputP(i, j) == Approx(expectedP(i, j)).epsilon(0.01));
        }
    }
}

TEST_CASE("MEKF correct test - With eclipse") {
    Matrix<float, LocalStateSize, LocalStateSize> Q;
    Q << 1, 0, 0, 0, 0, 0,
            0, 1, 0, 0, 0, 0,
            0, 0, 1, 0, 0, 0,
            0, 0, 0, 1e-3, 0, 0,
            0, 0, 0, 0, 1e-3, 0,
            0, 0, 0, 0, 0, 1e-3;
    Q *= 1e-4;

    Matrix<float, MeasurementSize, MeasurementSize> R;
    R << 0.000000100000000, 0, 0, 0, 0, 0,
            0, 0.000000100000000, 0, 0, 0, 0,
            0, 0, 0.000000100000000, 0, 0, 0,
            0, 0, 0, 1.000000000000000, 0, 0,
            0, 0, 0, 0, 1.000000000000000, 0,
            0, 0, 0, 0, 0, 1.000000000000000;

    MEKF mekf;
    mekf.setQ(Q);
    mekf.setR(R);

    Matrix<float, LocalStateSize, LocalStateSize> P;
    P
            << 0.001000532028826, 0.000039799843243, -0.000023397035243, -0.000031228978606, -0.000000275326053, 0.000000439784453,
            0.000039799843243, 0.001308555447659, -0.000085623932552, -0.000003047889606, -0.000040206576649, 0.000002088616037,
            -0.000023397035243, -0.000085623932552, 0.001009595788290, 0.000001188949449, 0.000002693939586, -0.000031290752345,
            -0.000031228978606, -0.000003047889606, 0.000001188949449, 0.000032666516885, 0.000000060978029, -0.000000024302848,
            -0.000000275326053, -0.000040206576649, 0.000002693939586, 0.000000060978029, 0.000032932810253, -0.000000075615809,
            0.000000439784453, 0.000002088616037, -0.000031290752345, -0.000000024302848, -0.000000075615809, 0.000032650244727;
    mekf.setP(P);

    GlobalStateVector currentState(-0.765115125919115, -0.474545758467044, 0.069502711228155, -0.429621391858752,
                                   -0.072204707065460, 0.123869142919400, -0.064575258483096);
    mekf.setGlobalState(currentState);

    MeasurementVector measurements(0.080507270942589, -0.989408379069875, 0.120787576975857, 0, 0, 0);
    Vector3f magneticFieldECI(1.0e+04 * 3.094327965270048, -1.0e+04 * 0.855794129136668, -1.0e+04 * 2.159163431159944);
    Vector3f sunPositionECI(-0.175412927368641, 0.918817554837861, 0.398297949025007);
    bool eclipse = true;
    Vector3f satPositionECI(1.0e+06 * 4.569033859736713, 1.0e+06 * 0.261242083257025, -1.0e+06 * 5.135087257503072);
    float albedo = 1.000567064204868e-10;

    mekf.correct(measurements, magneticFieldECI, sunPositionECI, eclipse, satPositionECI, albedo);

    GlobalStateVector expectedState(-0.763759607585108, -0.477159784616164, 0.067216485704839, -0.429501741342207,
                                    -0.072431881749339, 0.123846663072216, -0.064529456997382);
    Matrix<float, LocalStateSize, LocalStateSize> expectedP;
    expectedP
            << 0.000008282830359, -0.000102485744922, 0.000011750668220, -0.000000114684634, 0.000003160214479, -0.000000323212935,
            -0.000102485744922, 0.001283679714947, -0.000147170904163, 0.000001397218160, -0.000039581899152, 0.000004047691243,
            0.000011750668220, -0.000147170904163, 0.000016974107366, -0.000000160181840, 0.000004537982225, -0.000000467202722,
            -0.000000114684634, 0.000001397218160, -0.000000160181840, 0.000031690793902, -0.000000046261632, 0.000000007295405,
            0.000003160214479, -0.000039581899152, 0.000004537982225, -0.000000046261632, 0.000032917017383, -0.000000134038474,
            -0.000000323212935, 0.000004047691243, -0.000000467202722, 0.000000007295405, -0.000000134038474, 0.000031692982486;

    auto outputState = mekf.getGlobalState();
    auto outputP = mekf.getP();

    for (int i = 0; i < currentState.size(); i++) {
        REQUIRE(outputState(i) == Approx(expectedState(i)).epsilon(0.01));
    }
    for (int i = 0; i < LocalStateSize; i++) {
        for (int j = 0; j < LocalStateSize; j++) {
            REQUIRE(outputP(i, j) == Approx(expectedP(i, j)).epsilon(0.01));
        }
    }
}
