require File.join(File.dirname(__FILE__), 'spec_helper')

describe GMP::Z, "IO operations" do
  before do
    @z = GMP::Z(2)**71 - 1
    @file = File.join(File.dirname(__FILE__), "ecm.out")
  end

  before(:each) do
    @handle = File.open(@file, 'w')
  end

  after(:each) do
    FileUtils.rm @file
  end

  it "should be verbose-ish with :verbose => 1" do
    @z.ecm_factor(1_000_000, :verbose => 1, :os => @handle)
    lines = File.read @file
    lines.should match(/Using B1=1000000, B2=\d+, polynomial Dickson\(6\), sigma=\d+/)
    lines.should match(/Step 1 took \d+ms/)
  end

  # Using special division for factor of 2^71-1
  # Using B1=1000000, B2=1045563762, polynomial Dickson(6), sigma=1674549887
  # dF=4096, k=6, d=39270, d2=13, i0=13
  # Expected number of curves to find a factor of n digits:
  # 35  40  45  50  55  60  65  70  75  80
  # 910 8615  97096 1281819 1.9e+07 3.1e+08 6.2e+09 1.7e+11 2.3e+16 3.1e+21
  # Reached point at infinity, 578029 divides group order
  # Step 1 took 1210ms
  it "should be verbose-ish with :verbose => 2" do
    @z.ecm_factor(1_000_000, :verbose => 2, :os => @handle)
    lines = File.read @file
    lines.should match(/Using special division for factor of 2\^71-1/)
    lines.should match(/Using B1=1000000, B2=\d+, polynomial Dickson\(6\), sigma=\d+/)
    lines.should match(/Using dF=\d+, k=\d+, d=\d+, d2=\d+, i0=\d+/)
    lines.should match(/Step 1 took \d+ms/)
  end
end
