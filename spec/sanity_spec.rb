# I'm having trouble coming up with decent unit tests, so for now... sanity tests!
require File.join(File.dirname(__FILE__), 'spec_helper')

describe ECMParams, "new" do
  before do
    @z = GMP::Z(2)**255 - 1
    @z2 = GMP::Z("2050449353925555290706354283")
    @z3 = GMP::Z("212252637915375215854013140804296246361")
  end

  it "allows a missing ECMParams" do
    @z.ecm_factor(1_000_000)
  end

  it "allows a simple hash with :method" do
    @z.ecm_factor(1_000_000, :method => :ECM_ECM)
    @z.ecm_factor(1_000_000, :method => :ECM_PM1)
    @z.ecm_factor(1_000_000, :method => :ECM_PP1)
  end

  it "allows a simple hash with :x" do
    @z.ecm_factor(1_000_000, :z => GMP::Z(1_000))
  end

  it "allows a simple hash with :sigma" do
    @z2.ecm_factor 1_000_000, :sigma => 7
    @z2.ecm_factor 1_000_000, :sigma => GMP::Z(7)
  end

  it "allows a simple hash with :sigma_is_A" do
    @z2.ecm_factor 1_000_000, :sigma_is_A =>  1, :x => GMP::Z(7)
    @z2.ecm_factor 1_000_000, :sigma_is_A =>  0, :x => GMP::Z(7)
    #@z2.ecm_factor 1_000_000, :sigma_is_A => -1
  end

  it "allows a simple hash with :go" do
    @z3.ecm_factor 1_000_000, :sigma =>  GMP::Z(781683988), :go => GMP::Z(550232165123)
  end

  it "allows a simple hash with :B1done" do
    @z3.ecm_factor 1_000_000, :sigma =>  GMP::Z(781683988), :go => GMP::Z(550232165123), :B1done => 1000.0
  end

  it "allows a simple hash with :B2min" do
    @z2.ecm_factor 1_000_000, :B2min => 2_000_000
  end

  it "allows a simple hash with :B2min" do
    @z2.ecm_factor 1_000_000, :B2 => 2_000_000
  end
end

describe ECMParams, "bad types" do
  before do
    @z = GMP::Z(2)**255 - 1
  end

  it "blows up if sigma_is_A is not a Fixnum" do
    expect { @z1.ecm_factor 1_000_000, :sigma => 7, :sigma_is_A => 2.3 }.to raise_exception
  end
end
