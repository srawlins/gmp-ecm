require File.join(File.dirname(__FILE__), 'spec_helper')

describe GMP::Z, "file operations" do
  before do
    @z = GMP::Z("13155161912808540373988986448257115022677318870175067553764004308210487")
    @filename = File.join(File.dirname(__FILE__), "pm1chkpoint")
  end

  after(:each) do
    FileUtils.rm @filename
  end

  it "should be verbose-ish with :verbose => 1" do
    @z.ecm_factor(10_000_000, :method => :pm1, :chkfilename => @filename)
    lines = File.read @filename
    lines.should match(/B1=\d+; N=\d+; X=\d+/)
  end
end
