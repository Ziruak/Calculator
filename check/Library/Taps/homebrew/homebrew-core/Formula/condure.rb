class Condure < Formula
  desc "HTTP/WebSocket connection manager"
  homepage "https://github.com/fanout/condure"
  url "https://github.com/fanout/condure/archive/1.4.1.tar.gz"
  sha256 "6a30cf8f5aa51d958099c15433f23dacc1a75780b1aace9a94dce40a5e755d90"
  license "Apache-2.0"

  bottle do
    sha256 cellar: :any,                 arm64_monterey: "c29560884c7a1f3924f08f7e4485aba3dde2b1305fe1a1cd6ad6d1ef3fb0ca1d"
    sha256 cellar: :any,                 arm64_big_sur:  "bdaac9f52ee87fe0efdb4829510ccf3a0c786cd33c289961e5b4b75f8b5f7400"
    sha256 cellar: :any,                 monterey:       "89821560bbe16b7d91bf69681779d47855840ab11376bfcb836462b294883c2f"
    sha256 cellar: :any,                 big_sur:        "76081254666364aeb1cf4d360ba92bf33ebca8b9c2a3d34a1c3448a94efacf5c"
    sha256 cellar: :any,                 catalina:       "04b81800979389b4e9bcda0cd0d68dbcb48c1e9f52640c243ca94309a36cf124"
    sha256 cellar: :any_skip_relocation, x86_64_linux:   "1c2fd4b5ef3515f8411ff3211e8ffe4caf815b1384f87c66e27b265cbe7ba06b"
  end

  depends_on "pkg-config" => :build
  depends_on "rust" => :build
  depends_on "python@3.9" => :test
  depends_on "openssl@1.1"
  depends_on "zeromq"

  resource "pyzmq" do
    url "https://files.pythonhosted.org/packages/6c/95/d37e7db364d7f569e71068882b1848800f221c58026670e93a4c6d50efe7/pyzmq-22.3.0.tar.gz"
    sha256 "8eddc033e716f8c91c6a2112f0a8ebc5e00532b4a6ae1eb0ccc48e027f9c671c"
  end

  resource "tnetstring3" do
    url "https://files.pythonhosted.org/packages/d9/fd/737a371f539842f6fcece47bb6b941700c9f924e8543cd35c4f3a2e7cc6c/tnetstring3-0.3.1.tar.gz"
    sha256 "5acab57cce3693d119265a8ac019a9bcdc52a9cacb3ba37b5b3a1746a1c14d56"
  end

  def install
    system "cargo", "install", *std_cargo_args
  end

  test do
    ipcfile = testpath/"client"
    runfile = testpath/"test.py"

    resource("pyzmq").stage do
      system Formula["python@3.9"].opt_bin/"python3",
      *Language::Python.setup_install_args(testpath/"vendor")
    end

    resource("tnetstring3").stage do
      system Formula["python@3.9"].opt_bin/"python3",
      *Language::Python.setup_install_args(testpath/"vendor")
    end

    runfile.write(<<~EOS,
      import threading
      from urllib.request import urlopen
      import tnetstring
      import zmq
      def server_worker(c):
        ctx = zmq.Context()
        sock = ctx.socket(zmq.REP)
        sock.connect('ipc://#{ipcfile}')
        c.acquire()
        c.notify()
        c.release()
        while True:
          m_raw = sock.recv()
          req = tnetstring.loads(m_raw[1:])
          resp = {}
          resp[b'id'] = req[b'id']
          resp[b'code'] = 200
          resp[b'reason'] = b'OK'
          resp[b'headers'] = [[b'Content-Type', b'text/plain']]
          resp[b'body'] = b'test response\\n'
          sock.send(b'T' + tnetstring.dumps(resp))
      c = threading.Condition()
      c.acquire()
      server_thread = threading.Thread(target=server_worker, args=(c,))
      server_thread.daemon = True
      server_thread.start()
      c.wait()
      c.release()
      with urlopen('http://localhost:10000/test') as f:
        body = f.read()
        assert(body == b'test response\\n')
    EOS
                 )

    pid = fork do
      exec "#{bin}/condure", "--listen", "10000,req", "--zclient-req", "ipc://#{ipcfile}"
    end

    begin
      xy = Language::Python.major_minor_version Formula["python@3.9"].opt_bin/"python3"
      ENV["PYTHONPATH"] = testpath/"vendor/lib/python#{xy}/site-packages"
      system Formula["python@3.9"].opt_bin/"python3", runfile
    ensure
      Process.kill("TERM", pid)
      Process.wait(pid)
    end
  end
end
