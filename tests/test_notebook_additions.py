import json
from pathlib import Path
import shutil
import subprocess
import tempfile
import unittest


ROOT = Path(__file__).resolve().parents[1]
COMPILER = shutil.which('g++') or shutil.which('clang++')


@unittest.skipUnless(COMPILER, 'C++ compiler required')
class NotebookAdditionsTests(unittest.TestCase):
    def run_cpp(self, snippet, body):
        source = '#include <algorithm>\n#include <cassert>\n#include <numeric>\n#include <random>\n'
        source += '#define main notebook_main\n#include '
        source += json.dumps(str(ROOT / 'content' / snippet))
        source += '\n#undef main\n' + body
        with tempfile.TemporaryDirectory() as directory:
            root = Path(directory)
            (root / 'test.cpp').write_text(source)
            compiled = subprocess.run(
                [COMPILER, '-std=c++20', '-O2', str(root / 'test.cpp'), '-o', str(root / 'test')],
                capture_output=True, text=True
            )
            self.assertEqual(compiled.returncode, 0, compiled.stderr)
            result = subprocess.run([str(root / 'test')], capture_output=True, text=True, timeout=20)
            self.assertEqual(result.returncode, 0, result.stderr)

    def test_gauss_determinant_and_rank_against_minors(self):
        self.run_cpp('math/gaussian-elimination.cpp', r'''
using Mat = vector<vector<ll>>;
ll bruteDet(const Mat &a) {
    int n = a.size(); vi p(n); iota(all(p), 0); ll ans = 0;
    do {
        ll term = 1; int parity = 0;
        for (int i = 0; i < n; ++i) {
            term = term * a[i][p[i]] % mod;
            for (int j = 0; j < i; ++j) parity ^= p[j] > p[i];
        }
        ans = (ans + (parity ? -term : term)) % mod;
    } while (next_permutation(all(p)));
    return (ans + mod) % mod;
}
int bruteRank(const Mat &a, int n, int m) {
    int rank = 0;
    for (int rows = 1; rows < (1 << n); ++rows)
        for (int cols = 1; cols < (1 << m); ++cols) {
            int k = __builtin_popcount((unsigned)rows);
            if (k <= rank || k != __builtin_popcount((unsigned)cols)) continue;
            Mat minor;
            for (int i = 0; i < n; ++i) if (rows >> i & 1) {
                vector<ll> row;
                for (int j = 0; j < m; ++j) if (cols >> j & 1) row.push_back(a[i][j]);
                minor.push_back(row);
            }
            if (bruteDet(minor)) rank = k;
        }
    return rank;
}
int main() {
    Mat empty; ll det = -1;
    assert(gauss(empty, det) == 0 && det == 1);
    Mat swapRows = {{0,1},{1,0}};
    assert(gauss(swapRows, det) == 2 && det == mod - 1);
    Mat singular = {{1,2},{2,4}};
    assert(gauss(singular, det) == 1 && det == 0);
    mt19937 random(2026);
    for (int n = 1; n <= 5; ++n) for (int trial = 0; trial < 50; ++trial) {
        Mat a(n, vector<ll>(n));
        for (auto &row : a) for (ll &x : row) x = (ll(random() % 11) - 5 + mod) % mod;
        Mat b = a; gauss(b, det);
        assert(det == bruteDet(a));
    }
    for (int n = 1; n <= 4; ++n) for (int m = 0; m <= 5; ++m)
        for (int trial = 0; trial < 15; ++trial) {
            Mat a(n, vector<ll>(m));
            for (auto &row : a) for (ll &x : row) x = random() % 3;
            Mat b = a;
            int expected = bruteRank(a, n, m);
            assert(gauss(b, det) == expected);
            assert(det == (n == m ? bruteDet(a) : 0));
        }
}
''')

    def test_divisor_transforms_against_direct_sums(self):
        self.run_cpp('dsa/zeta.cpp', r'''
int main() {
    assert(divZeta(vi{0,1,2})[2] == 3);
    assert(mulZeta(vi{0,3,2},true)[1] == 1);
    mt19937 random(2026);
    for (int n = 0; n <= 100; ++n) for (int trial = 0; trial < 10; ++trial) {
        vi a(n + 1), ds(n + 1), ms(n + 1);
        for (int &x : a) x = int(random() % 101) - 50;
        ds[0] = ms[0] = a[0];
        for (int i = 1; i <= n; ++i) for (int j = 1; j <= n; ++j) {
            if (i % j == 0) ds[i] += a[j];
            if (j % i == 0) ms[i] += a[j];
        }
        assert(divZeta(a) == ds);
        assert(mulZeta(a) == ms);
        assert(divZeta(ds,true) == a);
        assert(mulZeta(ms,true) == a);
    }
}
''')

    def test_circle_polygon_area_analytic_and_clipping(self):
        self.run_cpp('geometry/circle-polygon-area.cpp', r'''
const ld PI = acosl(-1);
void near(ld a, ld b, ld eps = 1e-10L) { assert(abs(a-b) <= eps * (1+abs(b))); }
vector<Pt> box(ld l, ld b, ld r, ld t) { return {{l,b},{r,b},{r,t},{l,t}}; }
// Independent reference: clip a regular 4096-gon approximating the disk
// against the four half-planes of a convex rectangle.
ld reference(const vector<Pt> &rect, Pt c, ld r) {
    vector<Pt> p;
    for (int i = 0; i < 4096; ++i) p.push_back(c + polar(r, 2*PI*i/4096));
    for (int i = 0; i < 4; ++i) {
        Pt a = rect[i], d = rect[(i+1)%4] - a;
        vector<Pt> q;
        for (int j = 0, n = p.size(); j < n; ++j) {
            Pt u = p[j], v = p[(j+1)%n];
            ld x = cross(d,u-a), y = cross(d,v-a);
            if (x >= 0) q.push_back(u);
            if ((x < 0) != (y < 0)) q.push_back(u + (v-u) * (x/(x-y)));
        }
        p = q;
    }
    ld area = 0;
    for (int i = 0, n = p.size(); i < n; ++i) area += cross(p[i],p[(i+1)%n]);
    return abs(area)/2;
}
int main() {
    near(circlePolyArea({}, {0,0}, 1), 0);
    near(circlePolyArea(box(-2,-2,2,2), {0,0}, 0), 0);
    near(circlePolyArea(box(-2,-2,2,2), {0,0}, 1), PI);
    near(circlePolyArea(box(-.25L,-.25L,.25L,.25L), {0,0}, 1), .25L);
    near(circlePolyArea(box(0,0,2,2), {0,0}, 1), PI/4);
    near(circlePolyArea(box(0,-2,2,2), {0,0}, 1), PI/2);
    near(circlePolyArea(box(1,-1,3,1), {0,0}, 1), 0); // tangent at edge midpoint
    near(circlePolyArea(box(2,2,3,3), {0,0}, 1), 0);
    vector<Pt> concave = {{-2,-2},{2,-2},{2,0},{0,0},{0,2},{-2,2}};
    near(circlePolyArea(concave, {0,0}, 1), 3*PI/4);
    concave.insert(concave.begin(), concave[0]);
    concave.push_back(concave[0]);
    reverse(concave.begin(),concave.end());
    near(circlePolyArea(concave, {0,0}, 1), 3*PI/4);
    for (auto &p : concave) p = 3.L*p + Pt(7,-5);
    near(circlePolyArea(concave, {7,-5}, 3), 27*PI/4);
    mt19937 random(2026);
    uniform_real_distribution<ld> pos(-2,2), size(.05L,2), angle(0,2*PI);
    for (int trial = 0; trial < 100; ++trial) {
        ld w = size(random), h = size(random), r = size(random);
        Pt center(pos(random),pos(random)), shift(pos(random),pos(random));
        Pt rotation = polar(1.L,angle(random));
        auto p = box(-w,-h,w,h);
        for (Pt &v : p) v = v * rotation + shift;
        ld got = circlePolyArea(p,center,r);
        near(got,reference(p,center,r),2e-5L);
        reverse(p.begin(),p.end());
        near(circlePolyArea(p,center,r),got);
    }
}
''')


if __name__ == '__main__':
    unittest.main()
